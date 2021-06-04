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
           :min-height 1140
           :resizable true
           :scene {:fx/type :scene
                   :fill ((fx/sub-val context :colors) :background-color)
                   :stylesheets [(::css/url (fx/sub-val context :style-sheet))]
                   :root {:fx/type :v-box
                          :children [{:fx/type :menu-bar
                                      :max-height 10 ;somehow make the height actually work
                                      :style-class "root-menu-bar"
                                      :menus [{:fx/type :menu
                                               :text "file"
                                               :style-class "root-menu-bar-item"
                                               :items [{:fx/type :menu-item
                                                        :style-class "root-menu-bar-item-sub-item"
                                                        :text "open"
                                                        :on-action {:event/type :bettercode.events/openfex
                                                                    :tclient tclient}}
                                                       {:fx/type :menu-item
                                                        :style-class "root-menu-bar-item-sub-item"
                                                        :text "save"
                                                        :on-action {:event/type :bettercode.events/saveevent
                                                                    :tclient tclient}}
                                                       {:fx/type :menu-item
                                                        :style-class "root-menu-bar-item-sub-item"
                                                        :text "close file"
                                                        :on-action {:event/type :bettercode.events/close-file
                                                                    :tclient tclient}}]}
                                              {:fx/type :menu
                                               :text "theme"
                                               :style-class "root-menu-bar-item"
                                               :items [{:fx/type :menu-item
                                                        :style-class "root-menu-bar-item-sub-item"
                                                        :text "new theme"
                                                        :on-action {:event/type :bettercode.events/open-creator}}
                                                       {:fx/type :menu-item
                                                        :style-class "root-menu-bar-item-sub-item"
                                                        :text "existing theme"}]}]}
                                     {:fx/type bettercode.elements/editor-pane
                                      :tclient tclient
                                      :text ""
                                      :file-path "BetterCode"
                                      :style-class "root"}]}}}
          {:fx/type :stage
           :title "Files"
           :showing (fx/sub-val context :file-explorer-show)
           :width 500
           :height 350
           :resizable false
           :always-on-top true
           :modality :application-modal
           :scene {:fx/type :scene
                   :fill ((fx/sub-val context :colors) :background-color)
                   :stylesheets [(::css/url (fx/sub-val context :style-sheet))]
                   :root {:fx/type bettercode.utilelements/file-window
                          :tclient tclient}}}
          {:fx/type :stage
           :title "Theme"
           :showing (fx/sub-val context :theme-creator-show)
           :width 700
           :height 380
           :resizable false
           :scene {:fx/type :scene
                   :fill ((fx/sub-val context :colors) :background-color)
                   :stylesheets [(::css/url (fx/sub-val context :style-sheet))]
                   :root {:fx/type bettercode.utilelements/theme-creator}}}]})

;create file opening screen
(defn -main [hostname & args]
  (Platform/setImplicitExit true)
  (println "started")
  (let [c @(client (if hostname hostname "localhost") 8080)
        msg @(s/put! c ["get-dir"])
        dirs (vec @(s/take! c))
        *context
        (atom
         (fx/create-context {:title "BetterCode"
                             :file-path ""
                             :text-editor ""
                             :dir-contents dirs
                             :cur-path (bettercode.events/parent-dir (subs (dirs 0) 5))
                             :file-explorer-show true
                             :theme-creator-show false
                             :theme-name-entered ""
                             :file-name-entered ""
                             :line-numbers ""
                             :vscroll 0
                             :style-sheet bettercode.css/style
                             :colors bettercode.css/colors}
                            #(cache/lru-cache-factory % :threshold 4096)))]
    (fx/create-app *context
                   :event-handler bettercode.events/handle-event
                   :desc-fn (fn [_]
                              {:fx/type app
                               :tclient c}))))