(ns bettercode.events
  (:require [cljfx.api :as fx]
            [clojure.data :as data]
            [manifold.stream :as s]))

(defmulti handle-event :event/type)

(defmethod handle-event :default [e]
  (println e))

(defmethod handle-event ::type-text [{:keys [fx/event fx/context tclient]}]
  (println (. event getCharacter))
  @(s/put! tclient (. event getCharacter))
  {:context (fx/swap-context context
                             assoc
                             :argument (. event getCharacter)
                             :action "text-edit")})