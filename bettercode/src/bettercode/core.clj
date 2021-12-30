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
            [bettercode.utilelements]
            [bettercode.images]
            [bettercode.stages]
            [bettercode.utils])
  (:import [javafx.application Platform]
           [javafx.scene.image Image]))

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
   :desc [{:fx/type bettercode.stages/main-app
           :tclient tclient}
          {:fx/type bettercode.stages/file-explorer
           :tclient tclient}
          {:fx/type bettercode.stages/theme-creator
           :tclient tclient}
          {:fx/type bettercode.stages/theme-picker
           :tclient tclient}]})

(defn -main [hostname & args]
  (Platform/setImplicitExit true)
  (println "started")
  (let [c @(client (if hostname hostname "localhost") 8080) ;allow connecting to multiple servers later on
        msg @(s/put! c ["get-dir"])
        dirs (vec @(s/take! c))
        theme-dir-contents [] ;use raynes fs
        *context
        (atom
         (fx/create-context {:title "BetterCode"
                             :file-path ""
                             :text-editor ""
                             :dir-contents dirs
                             :cur-path (bettercode.utils/parent-dir (subs (dirs 0) 5))
                             :file-explorer-show true
                             :theme-creator-show false
                             :theme-picker-show false
                             :theme-name-entered ""
                             :file-name-entered ""
                             :line-numbers ""
                             :vscroll 0
                             :cursor-pos 0
                             :anchor-pos 0
                             :style-sheet bettercode.css/style
                             :colors bettercode.css/colors
                             :images {:file @(fx/on-fx-thread (Image. bettercode.images/file-icon))
                                      :folder @(fx/on-fx-thread (Image. bettercode.images/folder-icon))}
                             :themes (bettercode.meta/get-themes)
                             :spans [{:start 0 :end 0 :style "text-color"}]}
                            #(cache/lru-cache-factory % :threshold 4096)))]
    (fx/create-app *context
                   :event-handler (-> bettercode.events/event-handler
                                      (fx/wrap-co-effects
                                       {:fx/context (fx/make-deref-co-effect *context)}))
                   :desc-fn (fn [_]
                              {:fx/type app
                               :tclient c}))))
