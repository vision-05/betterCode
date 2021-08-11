(ns bettercode.events
  (:require [cljfx.api :as fx]
            [cljfx.css :as css]
            [clojure.data :as data]
            [clojure.edn :as edn]
            [clojure.string]
            [manifold.stream :as s]
            [bettercode.css]
            [bettercode.meta]
            [bettercode.handlers])
  )

(defn map-run
  "function that runs the corresponding fn in the event handler map with the correct args"
  [atom-map key args]
  ((@atom-map key) args))

(defn validate-handler [handler]
  true) ;make sure handler returns a map

(def event-handlers (atom (reduce-kv (fn [m k v] (assoc m (keyword k) v)) {} (ns-publics 'bettercode.handlers)))) ;store event handlers in a map. this is as atom so changeable at runtime, allowing custom handlers

(defn event-handler
  "function that calls the actual event handling functions and passes the arg map"
  [{:keys [fx/event fx/context event/type key tclient]}]
  (map-run event-handlers type {:event event :context context :key key :tclient tclient}))
