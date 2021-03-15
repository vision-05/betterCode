(ns bettercode.elements
  (:require [cljfx.api :as fx]))

(defn spacer [{:keys [width height]}]
  {:fx/type :rectangle
   :width width
   :height height
   :fill "#23282D"})

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
           :-fx-background-color "#23282D"
           :-fx-text-fill "#E0CDCD"}
   :on-text-changed {:event/type ::type-text :fx/sync true}})

(defn line-numbers [{:keys [fx/context]}]
  {:fx/type :text-area
   :min-width 80
   :min-height 896
   :max-width 80
   :max-height 896
   :style {:-fx-control-inner-background "#275F77"
           :-fx-background-color "#23282D"
           :-fx-text-fill "#E0CDCD"}})

(defn status-row [{:keys [fx/context]}]
  {:fx/type :h-box
   :max-width 768
   :children [{:fx/type spacer
               :width 14
               :height 32}
              {:fx/type status-bar}
              {:fx/type spacer
               :width 14
               :height 32}]})

(defn editor-row [{:keys [fx/context]}]
  {:fx/type :h-box
   :max-width 768
   :children [{:fx/type spacer
               :width 14
               :height 896}
              {:fx/type line-numbers}
              {:fx/type spacer
               :width 28
               :height 896}
              {:fx/type text-edit}
              {:fx/type spacer
               :width 14
               :height 896}]})

(defn editor-pane [{:keys [fx/context]}]
  "An editor with line numbers in an hbox layout"
  {:fx/type :v-box
   :max-width 768
   :max-height 1080
   :children [{:fx/type spacer
               :height 30
               :width 768}
              {:fx/type status-row}
              {:fx/type spacer
               :height 26
               :width 768}
              {:fx/type editor-row}
              {:fx/type spacer
               :height 96
               :width 768}]})