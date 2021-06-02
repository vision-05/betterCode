(ns bettercode.events
  (:require [cljfx.api :as fx]
            [clojure.data :as data]
            [manifold.stream :as s]))

(defmulti handle-event :event/type)

(defmethod handle-event :default [e]
  (println "non event"))

(defn parent-dir [path]
  (clojure.string/join "/" (drop-last (clojure.string/split path #"/"))))

(defn fex-back [context tclient]
  (let [cur-dir (fx/sub-val context :cur-path)
        prev-dir (parent-dir cur-dir)
        msg @(s/put! tclient ["get-dir" prev-dir]) ;get parent from :cur-path
        result @(s/take! tclient)
        dir-contents (if (= result true) @(s/take! tclient) result)]
    {:context (fx/swap-context context
                               assoc
                               :dir-contents dir-contents
                               :cur-path prev-dir)}))

(defn get-current-text [event]
  (-> event
      (.getSource)
      (.getParentPopup)
      (.getOwnerWindow)
      (.getScene)
      (.getRoot)
      (.getChildren)
      (.get 1)
      (.getChildren)
      (.get 1)
      (.getChildren)
      (.get 1)
      (.getText)))

;TODO: send event for replacing highlighted text, send event for inserting text

(defmethod handle-event ::scroll [{:keys [fx/event fx/context]}]
  {:context (fx/swap-context context
                             assoc
                             :vscroll (.getScrollTop (.getSource event)))})

(defmethod handle-event ::type-filename [{:keys [fx/event fx/context]}]
  {:context (fx/swap-context context
                             assoc
                             :file-name-entered (.getCharacters (.getSource event)))})

(defmethod handle-event ::newclick [{:keys [fx/event fx/context tclient]}]
  (let [file-name (fx/sub-val context :file-name-entered)
        file-path-new (str (fx/sub-val context :cur-path) "/" (.toString file-name))
        msg @(s/put! tclient ["open-file" file-path-new])
        foo @(s/take! tclient)] ;open file will return a value even if we don't use it
    {:context (fx/swap-context context ;how the fuck do I do this
                               assoc
                               :file-path file-path-new
                               :text-editor ""
                               :file-explorer-show false
                               :line-numbers "1\n")}))

(defmethod handle-event ::text-type [{:keys [fx/event fx/context]}]
  (let [no-of-lines (+ 2 (count (re-seq #"\n" (.getText (.getSource event)))))
        line-numbers (apply str (map #(str % \newline) (range 1 no-of-lines)))]
    {:context (fx/swap-context context
                               assoc
                               :line-numbers line-numbers
                               :vscroll (.getScrollTop (.getSource event)))}))

(defmethod handle-event ::openfex [{:keys [fx/event fx/context tclient]}]
  (if (not= (fx/sub-val context :file-path) "") (do
                                                  @(s/put! tclient ["update-buffer" (fx/sub-val context :file-path) (get-current-text event)])
                                                  (println "TAKING " @(s/take! tclient)))
      (println "NOT"))
  {:context (fx/swap-context context
                             assoc
                             :file-explorer-show true)})

(defmethod handle-event ::backclick [{:keys [fx/event fx/context tclient]}]
  (fex-back context tclient))

(defmethod handle-event ::fexclick [{:keys [fx/event fx/context tclient]}]
  (let [entry (.getText (.getTarget event))
        entry-info (subs entry 0 5)
        entry-name (subs entry 5)]
    (cond
      (= entry-info "DIR: ") (do @(s/put! tclient ["get-dir" entry-name])
                                 (let [result @(s/take! tclient)
                                       dir-contents (if (= result true) @(s/take! tclient) result)
                                       foo (println dir-contents)]
                                   {:context (fx/swap-context context
                                                              assoc
                                                              :dir-contents dir-contents
                                                              :cur-path entry-name)}))
      (= entry-info "FIL: ") (do @(s/put! tclient ["open-file" entry-name])
                                 (let [result @(s/take! tclient)
                                       file-contents (if (= result true) @(s/take! tclient) result)
                                       foo (println file-contents)
                                       no-of-lines (+ 2 (count (re-seq #"\n" file-contents)))
                                       line-numbers (apply str (map #(str % \newline) (range 1 no-of-lines)))]
                                   {:context (fx/swap-context context
                                                              assoc
                                                              :file-path entry-name
                                                              :text-editor file-contents
                                                              :file-explorer-show false
                                                              :line-numbers line-numbers)})))))

(defmethod handle-event ::close-file [{:keys [fx/event fx/context tclient]}]
  @(s/put! tclient ["close-file" (fx/sub-val context :file-path)])
  {:context (fx/swap-context context
                             assoc
                             :text-editor ""
                             :line-numbers ""
                             :file-path ""
                             :vscroll 0)})

(defmethod handle-event ::saveevent [{:keys [fx/event fx/context tclient]}]
  @(s/put! tclient ["save-file" (fx/sub-val context :file-path) (get-current-text event)])
  (let [sync-file @(s/take! tclient)]
    {:context (fx/swap-context context
                               assoc
                               :text-editor
                               sync-file)}))

(defmethod handle-event ::save-all [{:keys [fx/event fx/context tclient]}]
  @(s/put! tclient ["save-all" (fx/sub-val context :file-path (get-current-text event))])
  @(s/take! tclient))