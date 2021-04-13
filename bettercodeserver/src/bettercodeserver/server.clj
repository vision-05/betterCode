(ns bettercodeserver.server
  (:require [aleph.tcp :as tcp]
            [manifold.stream :as s]
            [manifold.deferred :as d]
            [gloss.io :as io]
            [gloss.core :as gloss]
            [clojure.edn :as edn]
            [clojure.string :as str]
            [bettercodeserver.buffer :as buffer]
            [bettercodeserver.filenav :as fnav]))

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

(defn parse-request [message agent-name]
  (let [split-req (str/split message #" ")]
    (println message)
    (println (split-req 0))
    (case (split-req 0)
      "text-edit" (buffer/text-edit agent-name (split-req 1) (split-req 3) (split-req 2))
      "open-file" (buffer/add-file agent-name (split-req 1))
      "close-file" (buffer/remove-file agent-name (split-req 1))
      "save-file" (buffer/save-file agent-name (split-req 1))
      "save-all" (buffer/save-all-files agent-name)
      "get-dir" (fnav/get-folder-contents (split-req 1)))))

(defn event-loop [f files-agent]
  (fn [s info]
    (d/loop []
            (->
                (d/let-flow [msg (s/take! s ::none)]
                            (when-not (= ::none msg)
                              (d/let-flow [msg-two (d/future (f msg files-agent))
                                           result (cond
                                            (= (class "") (class msg-two)) @(s/put! s msg-two)
                                            :else @(s/put! s true))]
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