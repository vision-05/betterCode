(ns bettercode.events
  (:require [cljfx.api :as fx]
            [clojure.data :as data]
            [manifold.stream :as s]))

(defmulti handle-event :event/type)

(defmethod handle-event :default [e]
  (println "non event"))

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
                                                              :dir-contents dir-contents)}))
      (= entry-info "FIL: ") (do @(s/put! tclient ["open-file" entry-name])
                                 (let [file-contents @(s/take! tclient)]
                                   (println file-contents)
                                   {:context (fx/swap-context context
                                                              assoc
                                                              :file-path entry-name
                                                              :text-editor file-contents)})))))

(defmethod handle-event ::type-text [{:keys [fx/event fx/context tclient]}]
  (println event) ;get soruce of event, prefereably
  @(s/put! tclient ["text-edit" (fx/sub-val context :file-path) (.getCharacter event) (.getCaretPosition (.getSource event))])
  (println @(s/take! tclient))
  {:context context})

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