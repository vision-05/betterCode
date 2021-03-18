(ns bettercode.events
  (:require [cljfx.api :as fx]
            [clojure.data :as data]))

(defmulti handle-event :event/type)

(defmethod handle-event :default [e]
  (println e))

(defmethod handle-event ::type-text [{:keys [fx/event fx/context]}]
  {:context (fx/swap-context context
                             assoc
                             :text-editor event
                             :argument (data/diff event (fx/sub-val context :text-editor))
                             :action "text-edit")})