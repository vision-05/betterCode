(ns bettercodeserver.server
  (:require [aleph.tcp :as tcp]
            [manifold.stream :as s]
            [manifold.deferred :as d]
            [gloss.io :as io]
            [gloss.core :as gloss]
            [clojure.edn :as edn]
            [bettercodeserver.buffer :as buffer]))

(def protocol
  (gloss/compile-frame
   (gloss/finite-frame :uint32
                       (gloss/string :utf-8))
   pr-str
   edn/read-string))

(defn wrap-duplex-stream [protocol s]
  (let [out (s/stream)]
    (s/connect
     (s/map #(io/encode protocol %) out)
     s)
    (s/splice out
              (io/decode-stream s protocol))))

(defn event-loop [f]
  (fn [s info]
    (d/loop []
            (->
                (d/let-flow [msg (s/take! s ::none)]
                            (when-not (= ::none msg)
                              (d/let-flow [msg-two (d/future (f msg))
                                           result (s/put! s msg-two)]
                                          (when result
                                            (d/recur)))))
                (d/catch
                 (fn [exception]
                   (s/put! s (str "ERROR: " exception))
                   (s/close! s)))))))

(defn start-server [handler port]
  (tcp/start-server
   (fn [s info]
     (handler (wrap-duplex-stream protocol s) info))
   {:port port}))