(ns bettercodeserver.core
  (:require [manifold.stream :as s]
            [bettercodeserver.server :as server]
            [bettercodeserver.buffer :as buffer])
  (:gen-class))

(defn -main []
  (def files (agent {}))
  (def serv (server/start-server
              (server/event-loop server/handle-events)
              8080))
  (.close serv))