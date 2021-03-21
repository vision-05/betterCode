(ns bettercode.elements
  (:require [cljfx.api :as fx]
            [bettercode.css]
            [bettercode.events]))

(defn spacer [{:keys [width height]}]
  {:fx/type :rectangle
   :width width
   :height height
   :fill "#23282D"})

(defn status-bar [{:keys [fx/context]}]
  {:fx/type :label
   :pref-width 740
   :pref-height 32
   :max-width 740
   :max-height 32
   :alignment :center
   :text (fx/sub-val context :file-name)
   :style-class "root-text-area-status"})

(defn text-edit [{:keys [fx/context tclient]}] ;put in scroll pane and hide/disable scroll bars
  {:fx/type :text-area
   :pref-width 632
   :pref-height 896
   :max-width 632
   :max-height 896
   :wrap-text true
   :font "Roboto Mono"
   :cursor :text
   :text (fx/sub-val context :text-editor)
   :style-class "root-text-area-editor"
   :on-key-typed {:event/type :bettercode.events/type-text
                  :fx/sync true
                  :tclient tclient}})

(defn line-numbers [{:keys [fx/context]}]
  {:fx/type :text-area
   :editable false
   :pref-width 80
   :pref-height 896
   :max-width 80
   :max-height 896
   :font "Roboto Mono"
   :style-class "root-text-area-numbers"})

(defn status-row [{:keys [fx/context]}]
  {:fx/type :h-box
   :pref-width 768
   :min-width 768
   :max-width 768
   :children [{:fx/type spacer
               :width 14
               :height 32}
              {:fx/type status-bar}
              {:fx/type spacer
               :width 14
               :height 32
               :h-box/hgrow :always}]})

(defn editor-row [{:keys [fx/context tclient]}]
  {:fx/type :h-box
   :pref-width 768
   :min-width 768
   :max-width 768
   :children [{:fx/type spacer
               :width 14
               :height 896}
              {:fx/type line-numbers}
              {:fx/type spacer
               :width 28
               :height 896}
              {:fx/type text-edit
               :tclient tclient}
              {:fx/type spacer
               :width 14
               :height 896
               :h-box/hgrow :always}]})

(defn editor-pane [{:keys [fx/context tclient]}]
  {:fx/type :v-box
   :pref-width 768
   :min-width 768
   :max-width 768
   :pref-height 1080
   :children [{:fx/type spacer
               :height 30
               :width 768}
              {:fx/type status-row}
              {:fx/type spacer
               :height 26
               :width 768
               :v-box/vgrow :always}
              {:fx/type editor-row
               :tclient tclient}
              {:fx/type spacer
               :height 96
               :width 768}]})