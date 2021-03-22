(ns bettercode.core
  (:gen-class)
  (:require [cljfx.api :as fx]
            [clojure.core.cache :as cache]
            [cljfx.css :as css]
            [bettercode.elements]
            [bettercode.css]
            [bettercode.events]
            [aleph.tcp :as tcp]
            [manifold.deferred :as d]
            [manifold.stream :as s]
            [clojure.edn :as edn]
            [gloss.core :as gloss]
            [gloss.io :as io]))

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

(defn client [host port]
  (d/chain (tcp/client {:host host :port port})
           #(wrap-duplex-stream protocol %)))

(def *context
  (atom
   (fx/create-context {:title "BetterCode"
                       :file-name "foo.txt"
                       :text-editor (slurp "/home/tim/foo.txt")
                       :anchor-pos 0
                       :caret-pos 0}
                      #(cache/lru-cache-factory % :threshold 4096))))

(defn -main []
  (let [c @(client "tim-archpc" 7000)]
    (println @(s/put! c "made connection"))
    (fx/create-app *context
                   :event-handler bettercode.events/handle-event
                   :desc-fn (fn [_]
                              {:fx/type :stage
                               :title "BetterCode"
                               :showing true
                               :width 768
                               :height 1080
                               :min-width 768
                               :min-height 1080
                               :resizable true
                               :scene {:fx/type :scene
                                       :fill "#23282D"
                                       :stylesheets [(::css/url bettercode.css/style)]
                                       :root {:fx/type bettercode.elements/editor-pane
                                              :tclient c
                                              :style-class "root"}}}))))