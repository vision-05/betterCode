(ns bettercode.events
  (:require [cljfx.api :as fx]
            [clojure.data :as data]
            [manifold.stream :as s]))

(defmulti handle-event :event/type)

(defmethod handle-event :default [e]
  (println e))

(defmethod handle-event ::type-text [{:keys [fx/event fx/context tclient]}]
  (println (.getCaretPosition (.getTarget event)))
  @(s/put! tclient "text-edit")
  @(s/put! tclient (.getCaretPosition (.getTarget event)))
  @(s/put! tclient (.getAnchor (.getTarget event))) ;this does not give the correct anchor,
  @(s/put! tclient (.getCharacter event))           ;store anchor in the context for correct
  {:context context})                               ;one

(defmethod handle-event ::highlight-text [{:keys [fx/event fx/context tclient]}]
  (println "highlighting"))

(defmethod handle-event ::move-caret [{:keys [fx/event fx/context tclient]}]
  (println "moving caret"))

(defmethod handle-event ::open-file [{:keys [fx/event fx/context tclient]}]
  (println "getting file")
  @(s/take! tclient))

(defmethod handle-event ::close-file [{:keys [fx/event fx/context tclient]}]
  (println "requesting close file"))

(defmethod handle-event ::save-file [{:keys [fx/event fx/context tclient]}]
  (println "saving file"))

(defmethod handle-event ::save-all [{:keys [fx/event fx/context tclient]}]
  (println "saving all buffers"))