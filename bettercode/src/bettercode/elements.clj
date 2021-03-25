(ns bettercode.elements
  (:require [cljfx.api :as fx]
            [bettercode.css]
            [bettercode.events])
  (:import (org.fxmisc.richtext CodeArea)))

(defn status-bar [{:keys [fx/context]}]
  {:fx/type :text-field
   :min-height 32
   :max-height 32
   :min-width 740
   :editable false
   :alignment :center
   :text (fx/sub-val context :file-name)
   :style-class "root-text-area-status"})

(defn text-edit [{:keys [fx/context tclient]}]
  {:fx/type fx/ext-instance-factory ;figure out how to set style
   :create #(doto (CodeArea. (slurp "/home/tim/foo.txt"))
              (.setPrefWidth 632)
              (.setPrefHeight 896))})

;(defn text-edit [{:keys [fx/context tclient]}]
;  {:fx/type StyleClassedTextArea
;   :pref-width 632
;   :pref-height 896
;   :font "Roboto Mono"
;   :cursor :text
;   :text (fx/sub-val context :text-editor)
;   :style-class "root-text-area-editor"
;   :on-key-typed {:event/type :bettercode.events/type-text
;                  :fx/sync true
;                  :tclient tclient}
;   :on-mouse-clicked {:event/type :bettercode.events/mouse-click
;                      :fx/sync true
;                      :tclient tclient}})

(defn line-numbers [{:keys [fx/context]}]
  {:fx/type :text-area
   :editable false
   :pref-width 80
   :pref-height 896
   :font "Roboto Mono"
   :style-class "root-text-area-numbers"})

(defn status-row [{:keys [fx/context]}]
  {:fx/type :h-box
   :min-width 768
   :padding 14
   :children [{:fx/type status-bar
               :h-box/hgrow :always}]})

(defn editor-row [{:keys [fx/context tclient]}]
  {:fx/type :h-box
   :pref-width 768
   :min-width 768
   :padding 14
   :spacing 28
   :children [{:fx/type line-numbers
               :h-box/vgrow :always}
              {:fx/type text-edit
               :tclient tclient
               :h-box/vgrow :always
               :h-box/hgrow :always}]})

(defn editor-pane [{:keys [fx/context tclient]}]
  {:fx/type :v-box
   :pref-width 768
   :min-width 768
   :pref-height 1080
   :min-height 1080
   :children [{:fx/type status-row
               :v-box/hgrow :always}
              {:fx/type editor-row
               :tclient tclient
               :v-box/vgrow :always
               :v-box/hgrow :always}]})