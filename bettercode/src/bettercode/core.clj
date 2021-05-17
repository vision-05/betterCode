(ns bettercode.core
  (:gen-class)
  (:require [cljfx.api :as fx]
            [cljfx.css :as css]
            [aleph.tcp :as tcp]
            [manifold.deferred :as d]
            [manifold.stream :as s]
            [gloss.core :as gloss]
            [gloss.io :as io]
            [clojure.core.cache :as cache]
            [clojure.edn :as edn]
            [bettercode.elements]
            [bettercode.css]
            [bettercode.events]
            [bettercode.utilelements])
  (:import [javafx.application Platform]))

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

(defn app [{:keys [fx/context tclient]}]
  {:fx/type fx/ext-many
   :desc [{:fx/type :stage
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
                          :tclient tclient
                          :text ""
                          :file-path "BetterCode"
                          :style-class "root"}}}
          {:fx/type :stage
           :title "Files"
           :showing (fx/sub context :file-explorer-show)
           :width 500
           :height 350
           :resizable false
           :scene {:fx/type :scene
                   :fill "#23282D"
                   :stylesheets [(::css/url bettercode.css/style)]
                   :root {:fx/type bettercode.utilelements/file-window
                          :tclient tclient}}}]})

;create file opening screen
(defn -main [hostname & args]
  (Platform/setImplicitExit true)
  (let [c @(client (if hostname hostname "localhost") 8080)
        msg @(s/put! c ["get-dir"])
        dirs @(s/take! c)
        *context
        (atom
         (fx/create-context {:title "BetterCode"
                             :file-path "/home/tim/foo.txt"
                             :text-editor ""
                             :dir-contents dirs
                             :cur-path "/home/tim/foo.txt"
                             :file-explorer-show true}
                            #(cache/lru-cache-factory % :threshold 4096)))]
    (fx/create-app *context
                   :event-handler bettercode.events/handle-event
                   :desc-fn (fn [_]
                              {:fx/type app
                               :tclient c}))))