(ns bettercode.core
  (:gen-class)
  (:require [cljfx.api :as fx]
            [clojure.core.cache :as cache]
            [cljfx.css :as css]
            [bettercode.elements]))

(def *context
  (atom
   (fx/create-context {:title "BetterCode"
                       :file-name "TextEditor.cpp"
                       :text-editor (slurp "/home/tim/betterCode/Server/TextEditor.cpp")
                       :action ""
                       :argument "hehe"}
                      #(cache/lru-cache-factory % :threshold 4096))))

(defn -main []
  (fx/create-app *context
                 :event-handler bettercode.elements/handle-event
                 :desc-fn (fn [_]
                            {:fx/type :stage
                             :title "BetterCode"
                             :showing true
                             :width 768
                             :height 1080
                             :resizable true
                             :scene {:fx/type :scene
                                     :fill "#23282D"
                                     :stylesheets [(::css/url bettercode.elements/style)]
                                     :root {:fx/type bettercode.elements/editor-pane
                                            :style-class "root"}}})))