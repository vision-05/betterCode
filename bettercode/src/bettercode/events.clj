(ns bettercode.events
  (:require [cljfx.api :as fx]
            [clojure.data :as data]
            [manifold.stream :as s]))

(defmulti handle-event :event/type)

(defmethod handle-event :default [e]
  (println "non event"))

(defmethod handle-event ::type-text [{:keys [fx/event fx/context tclient]}]
  @(s/put! tclient (str "text-edit " (fx/sub-val context :file-path) " " (fx/sub-val context :caret-pos) " " (.getCharacter event)))
  {:context (fx/swap-context context
                             assoc
                             :anchor-pos (.getAnchor (.getSource event))
                             :caret-pos (.getCaretPosition (.getSource event)))})


(defmethod handle-event ::mouse-click [{:keys [fx/event fx/context tclient]}]
  (println "click")
  (println (.getSource event))
  {:context (fx/swap-context context
                             assoc
                             :anchor-pos (.getAnchor (.getSource event))
                             :caret-pos (.getCaretPosition (.getSource event)))})

(defmethod handle-event ::open-file [{:keys [fx/event fx/context tclient file-path]}]
  (println "getting file")
  @(s/put! tclient (str "open-file " file-path))
  {:context (fx/swap-context (fx/swap-context context
                                              assoc
                                              :text-editor
                                              @(s/take! tclient))
                             assoc
                             :file-name
                             file-path)})

(defmethod handle-event ::close-file [{:keys [fx/event fx/context tclient]}]
  (println "requesting close file")
  @(s/put! tclient (str "close-file " (fx/sub-val context :file-path))))

(defmethod handle-event ::save-file [{:keys [fx/event fx/context tclient]}]
  (println "saving file")
  @(s/put! tclient (str "save-file " (fx/sub-val context :file-path)))
  @(s/take! tclient))

(defmethod handle-event ::save-all [{:keys [fx/event fx/context tclient]}]
  (println "saving all buffers")
  @(s/put! tclient "save-all")
  @(s/take! tclient))