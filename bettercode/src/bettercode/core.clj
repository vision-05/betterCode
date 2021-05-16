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

(def c @(client "localhost" 8080))
(def msg @(s/put! c ["get-dir"]))
(def dirs @(s/take! c))

(def *context
  (atom
   (fx/create-context {:title "BetterCode"
                       :file-path "/home/tim/foo.txt"
                       :text-editor ""
                       :dir-contents dirs}
                      #(cache/lru-cache-factory % :threshold 4096))))

;create file opening screen
(defn -main []
  (Platform/setImplicitExit true)
  (fx/create-app *context
                 :event-handler bettercode.events/handle-event
                 :desc-fn (fn [_]
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
                                                    :tclient c
                                                    :text ""
                                                    :file-path "BetterCode"
                                                    :style-class "root"}}}
                                    {:fx/type :stage
                                     :title "Files"
                                     :showing true
                                     :width 500
                                     :height 350
                                     :resizable false
                                     :scene {:fx/type :scene
                                             :fill "#23282D"
                                             :stylesheets [(::css/url bettercode.css/style)]
                                             :root {:fx/type bettercode.utilelements/file-window
                                                    :tclient c}}}]})))