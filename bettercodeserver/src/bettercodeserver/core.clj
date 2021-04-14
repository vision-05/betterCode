(ns bettercodeserver.core
  (:require [manifold.stream :as s]
            [bettercodeserver.server :as server]
            [bettercodeserver.buffer :as buffer])
  (:gen-class))

(defn -main []
  (def files (agent {"/home/tim/foo.txt" ""}))
  (def serv (server/start-server
              (server/event-loop server/parse-request files)
              8080)))
