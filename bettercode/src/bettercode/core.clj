(ns bettercode.core
  (:gen-class)
  (:require [cljfx.api :as fx]
            [clojure.string :as str]
            [clojure.data :as data]
            [clojure.core.cache :as cache]
            [bettercode.elements]))

(def *context
  (atom 
    (fx/create-context {:title "BetterCode"
                        :file-name "TextEditor.cpp"
                        :text-editor (slurp "/home/tim/betterCode/Server/TextEditor.cpp")
                        :action ""
                        :argument "hehe"}
                       #(cache/lru-cache-factory % :threshold 4096))))

(defmulti handle-event :event/type)

(defmethod handle-event :default [e]
  (println e))

(defmethod handle-event ::type-text [{:keys [fx/event fx/context]}]
  {:context (fx/swap-context context 
                             assoc 
                             :text-editor event 
                             :argument (data/diff event (fx/sub-val context :text-editor))
                             :action "text-edit")})

(defn -main []
  (fx/create-app *context
    :event-handler handle-event
    :desc-fn (fn [_]
                {:fx/type :stage
                 :showing true
                 :width 768
                 :height 1080
                 :scene {:fx/type :scene
                         :root {:fx/type bettercode.elements/editor-pane}}})))