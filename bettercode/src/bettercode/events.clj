(ns bettercode.events
  (:require [cljfx.api :as fx]
            [clojure.data :as data]
            [manifold.stream :as s]))

(defmulti handle-event :event/type)

(defmethod handle-event :default [e]
  (println "non event"))

(defn fex-back [context tclient]
  (let [cur-dir (fx/sub-val context :cur-path)
        prev-dir (clojure.string/join "/" (drop-last (clojure.string/split cur-dir #"/")))
        msg @(s/put! tclient ["get-dir" prev-dir]) ;get parent from :cur-path
        dir-contents @(s/take! tclient)]
    {:context (fx/swap-context context
                               assoc
                               :dir-contents dir-contents
                               :cur-path prev-dir)}))

;TODO: send event for replacing highlighted text, send event for inserting text

(defmethod handle-event ::type-filename [{:keys [fx/event fx/context]}]
  {:context (fx/swap-context context
                             assoc
                             :file-name-entered (.getCharacters (.getSource event)))})

(defmethod handle-event ::newclick [{:keys [fx/event fx/context tclient]}]
  (let [file-name (fx/sub-val context :file-name-entered)
        file-path-new (str (fx/sub-val context :cur-path) "/" (.toString file-name))
        msg @(s/put! tclient ["open-file" file-path-new])]
    (println "CUR-PATH: " (fx/sub-val context :cur-path))
    {:context (fx/swap-context context ;how the fuck do I do this
                               assoc
                               :file-path file-path-new
                               :text-editor ""
                               :file-explorer-show false)}))

(defmethod handle-event ::openfex [{:keys [fx/event fx/context]}]
  {:context (fx/swap-context context
                             assoc
                             :file-explorer-show true)})

(defmethod handle-event ::saveevent [{:keys [fx/event fx/context tclient]}]
  @(s/put! tclient ["save-file" (fx/sub-val context :file-path)])
  {:context context})

(defmethod handle-event ::backclick [{:keys [fx/event fx/context tclient]}]
  (fex-back context tclient))

(defmethod handle-event ::fexclick [{:keys [fx/event fx/context tclient]}]
  (let [entry (.getText (.getTarget event))
        entry-info (subs entry 0 5)
        entry-name (subs entry 5)]
    (cond
      (= entry-info "DIR: ") (do @(s/put! tclient ["get-dir" entry-name])
                                 (let [dir-contents @(s/take! tclient)]
                                   (println dir-contents)
                                   {:context (fx/swap-context context
                                                              assoc
                                                              :dir-contents dir-contents
                                                              :cur-path entry-name)}))
      (= entry-info "FIL: ") (do @(s/put! tclient ["open-file" entry-name])
                                 (let [file-contents @(s/take! tclient)]
                                   (println file-contents)
                                   {:context (fx/swap-context context
                                                              assoc
                                                              :file-path entry-name
                                                              :text-editor file-contents
                                                              :file-explorer-show false)})))))

(defmethod handle-event ::type-text [{:keys [fx/event fx/context tclient]}]
  (println event) ;get soruce of event, prefereably
  (let [message ["text-edit" (fx/sub-val context :file-path) (.getCharacter event) (.getCaretPosition (.getSource event))]
        length (- (fx/sub-val context :anchor-pos) (fx/sub-val context :caret-pos))]
    (println length (conj message length))
    @(s/put! tclient (if (> length 0) (assoc (conj message length) 3 (.getAnchor (.getSource event))) message)))
  (println @(s/take! tclient))
  {:context (fx/swap-context context
                             assoc
                             :anchor-pos (.getAnchor (.getSource event))
                             :caret-pos (.getCaretPosition (.getSource event)))})

(defmethod handle-event ::mouse-click [{:keys [fx/event fx/context tclient]}]
  (println (:fx/event event))
  (println (.getSource event))
  {:context (fx/swap-context context
                             assoc
                             :anchor-pos (.getAnchor (.getSource event))
                             :caret-pos (.getCaretPosition (.getSource event)))})

(defmethod handle-event ::close-file [{:keys [fx/event fx/context tclient]}]
  (println "requesting close file")
  @(s/put! tclient ["close-file" (fx/sub-val context :file-path)]))

(defmethod handle-event ::save-file [{:keys [fx/event fx/context tclient]}]
  (println "saving file")
  @(s/put! tclient ["save-file" (fx/sub-val context :file-path)])
  @(s/take! tclient))

(defmethod handle-event ::save-all [{:keys [fx/event fx/context tclient]}]
  (println "saving all buffers")
  @(s/put! tclient ["save-all"])
  @(s/take! tclient))