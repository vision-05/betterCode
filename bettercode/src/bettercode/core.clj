(ns bettercode.core
  (:gen-class)
  (:require [cljfx.api :as fx]
            [cljfx.css :as css]
            [clojure.string :as str]
            [clojure.data :as data]
            [clojure.core.cache :as cache]))

;(def style
;  (css/register ::style
;    (let [text-color "#E0CDCD"
;          editor-color "#123F58"
;          status-color "#275F77"
;          background-color "#23282D"]
;      {".text" {:-fx-text-fill text-color
;                "-editor" {:-fx-background-color editor-color
;                           :-fx-control-inner-background editor-color
;                           :-fx-text-fill text-color}
;                "-status" {:-fx-background-color status-color
;                           :-fx-text-fill text-color}}
;       ".rect" {:-fx-background-color background-color}})))

(def *context
  (atom 
    (fx/create-context {:title "BetterCode"
                        :file-name "TextEditor.cpp"
                        :text-editor (slurp "/home/tim/betterCode/Server/TextEditor.cpp")
                        :action ""
                        :argument "hehe"}
                       #(cache/lru-cache-factory % :threshold 4096))))

(defn spacer [{:keys [width]}]
  {:fx/type :rectangle
   :width width
   :style {:-fx-background-color "#23282D"
           :-fx-control-inner-background "#23282D"}})

(defn spacer-vertical [{:keys [height]}]
  {:fx/type :rectangle
   :height height})

(defn status-bar [{:keys [fx/context]}]
  "A status bar that stores the current column, line and filename"
  {:fx/type :label
   :min-width 740
   :min-height 32
   :max-width 740
   :max-height 32
   :alignment :center
   :text (fx/sub-val context :file-name)
   :style {:-fx-background-color "#275F77"
           :-fx-text-fill "#E0CDCD"}})

(defn text-edit [{:keys [fx/context]}]
  "A text editing area that updates the text-editor context"
  {:fx/type :text-area
   :min-width 632
   :min-height 896
   :max-width 632
   :max-height 896
   :text (fx/sub-val context :text-editor)
   :style {:-fx-control-inner-background "#123F58"
           :-fx-text-fill "#E0CDCD"}
   :on-text-changed {:event/type ::type-text :fx/sync true}})

(defn line-numbers [{:keys [fx/context]}]
  {:fx/type :text-area
   :min-width 80
   :min-height 896
   :max-width 80
   :max-height 896
   :style {:-fx-control-inner-background "#275F77"
           :-fx-text-fill "#E0CDCD"}})

(defn status-row [{:keys [fx/context]}]
  {:fx/type :h-box
   :max-width 768
   :children [{:fx/type spacer
               :width 14}
              {:fx/type status-bar}
              {:fx/type spacer
               :width 14}]})

(defn editor-row [{:keys [fx/context]}]
  {:fx/type :h-box
   :max-width 768
   :max-height 1000
   :children [{:fx/type spacer
               :width 14}
              {:fx/type line-numbers}
              {:fx/type spacer
               :width 28}
              {:fx/type text-edit}
              {:fx/type spacer
               :width 14}]})

(defn editor-pane [{:keys [fx/context]}]
  "An editor with line numbers in an hbox layout"
  {:fx/type :v-box
   :max-width 768
   :max-height 1080
   :children [{:fx/type spacer-vertical
               :height 30}
              {:fx/type status-row}
              {:fx/type spacer-vertical
               :height 26}
              {:fx/type editor-row}]})

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
                         :root {:fx/type editor-pane}}})))