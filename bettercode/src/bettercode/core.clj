(ns bettercode.core
  (:gen-class)
  (:require [cljfx.api :as fx]
            [clojure.string :as str]
            [clojure.data :as data]
            [clojure.core.cache :as cache]))

(def *context
  (atom 
    (fx/create-context {:title "BetterCode"
                        :file-name "TextEditor.cpp"
                        :text-editor (slurp "/home/tim/betterCode/Server/TextEditor.cpp")
                        :action ""
                        :argument "hehe"}
                       #(cache/lru-cache-factory % :threshold 4096))))

(defn status-bar [{:keys [fx/context text-color fill-color]}]
  {:fx/type :label
   :text (fx/sub-val context :file-name)
   :style {:-fx-text-fill text-color
           :-fx-background-color fill-color}})

(defn text-edit [{:keys [fx/context text-color fill-color]}]
  {:fx/type :text-area
   :text (fx/sub-val context :text-editor)
   :style {:-fx-text-fill text-color
           :-fx-background-color fill-color}
   :on-text-changed {:event/type ::type-text :fx/sync true}})

(defmulti handle-event :event/type)

(defmethod handle-event :default [e]
  (println e))

(defmethod handle-event ::type-text [{:keys [fx/event fx/context]}]
  {:context (fx/swap-context context assoc :text-editor event 
                                           :argument (data/diff event (fx/sub-val context :text-editor)))})

(defn -main []
  (fx/create-app *context
    :event-handler handle-event
    :desc-fn (fn [_]
                {:fx/type :stage
                 :showing true
                 :scene {:fx/type :scene
                         :root {:fx/type :v-box
                         :padding 10
                         :children [{:fx/type status-bar
                                     :text-color "#E0CDCD"
                                     :fill-color "#275F77"}
                                    {:fx/type text-edit
                                     :text-color "#000000"
                                     :fill-color "#123F58"}]}}})))