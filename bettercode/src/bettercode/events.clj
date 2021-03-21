(ns bettercode.events
  (:require [cljfx.api :as fx]
            [clojure.data :as data]
            [manifold.stream :as s]))

(defmulti handle-event :event/type)

(defmethod handle-event :default [e]
  (println "non event"))

(defmethod handle-event ::type-text [{:keys [fx/event fx/context tclient]}]
  (println (.getCaretPosition (.getTarget event)))
  (println (.getAnchor (.getTarget event)))
  @(s/put! tclient "text-edit")
  @(s/put! tclient (fx/sub-val context :caret-pos))
  @(s/put! tclient (fx/sub-val context :anchor-pos))
  @(s/put! tclient (.getCharacter event))
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

(defmethod handle-event ::open-file [{:keys [fx/event fx/context tclient]}]
  (println "getting file")
  @(s/take! tclient))

(defmethod handle-event ::close-file [{:keys [fx/event fx/context tclient]}]
  (println "requesting close file"))

(defmethod handle-event ::save-file [{:keys [fx/event fx/context tclient]}]
  (println "saving file"))

(defmethod handle-event ::save-all [{:keys [fx/event fx/context tclient]}]
  (println "saving all buffers"))