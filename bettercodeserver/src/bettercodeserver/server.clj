(ns bettercodeserver.server
  (:require [aleph.tcp :as tcp]
            [aleph.netty]
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

(defn parse-request
  "This function parses the message and then calls the appropriate function from the command in the message"
  [message agent-name]
  (case (message 0)
    "text-edit" (buffer/text-edit agent-name (message 1) (message 2) (message 3) (if (= 5 (count message)) (message 4) nil))
    "open-file" (buffer/add-file agent-name (message 1))
    "close-file" (buffer/remove-file agent-name (message 1))
    "save-file" (buffer/save-file agent-name (message 1))
    "save-all" (buffer/save-all-files agent-name)
    "get-dir" (if (> (count message) 1) (fnav/get-folder-contents (message 1))
                     (fnav/get-folder-contents))))

(defn event-loop 
  "Main event loop for the text server. Pass in the event handler and the agent with the file state"
  [f files-agent]
  (fn [s info]
    (d/loop []
      (->
       (d/let-flow [msg (s/take! s ::none)]
                   (when-not (= ::none msg)
                     (d/let-flow [msg-two (d/future (f msg files-agent))
                                  result (cond
                                           (= (class "") (class msg-two)) @(s/put! s msg-two)
                                           (= clojure.lang.Cons (class msg-two)) @(s/put! s msg-two)
                                           (= clojure.lang.LazySeq (class msg-two)) @(s/put! s msg-two)
                                           (= clojure.lang.PersistentList (class msg-two)) @(s/put! s msg-two)
                                           :else @(s/put! s true))]
                                 (when result
                                   (prn (class msg-two))
                                   (prn @files-agent)
                                   (d/recur)))))
       (d/catch
        (fn [exception]
          (println agent-error exception)
          (s/put! s (str "ERROR: " exception))
          (s/close! s)))))))

(defn start-server
  "Function borrowed from Aleph TCP example that starts a server connected to duplex stream"
  [handler port]
  (tcp/start-server
   (fn [s info]
     (handler (wrap-duplex-stream protocol s) info))
   {:port port})) ;somehow get this ssl working