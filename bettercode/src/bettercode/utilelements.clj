(ns bettercode.utilelements
  (:require [cljfx.api :as fx]
            [manifold.stream :as s]
            [bettercode.events]))

(defn new-button [{:keys [fx/context tclient text event-type]}]
  {:fx/type :button
   :style-class "root-fsview-button"
   :on-mouse-clicked {:event/type event-type
                      :fx/sync true
                      :tclient tclient}
   :text text})

(defn button-layout [{:keys [fx/context tclient left-button right-button]}]
  {:fx/type :h-box
   :alignment :center-right
   :padding 5
   :spacing 10
   :children [left-button
              right-button]})

(defn file-view [{:keys [fx/context tclient]}]
  {:fx/type :list-view
   :items (fx/sub-val context :dir-contents)
   :style-class "root-fsview"
   :on-mouse-clicked {:event/type :select-file-click
                      :fx/sync true
                      :tclient tclient}})

(defn file-window [{:keys [fx/context tclient]}]
  {:fx/type :v-box
   :padding 2
   :spacing 4
   :children [{:fx/type file-view
               :tclient tclient}
              {:fx/type :text-field
               :style-class "root-fsview-filename-input"
               :on-key-typed {:event/type :type-input
                              :key :file-name-entered
                              :fx/sync true}}
              {:fx/type button-layout
               :tclient tclient
               :left-button {:fx/type new-button
                             :tclient tclient
                             :text "Back"
                             :event-type :back-file-click}
               :right-button {:fx/type new-button
                              :tclient tclient
                              :text "New"
                              :event-type :new-file-click}}]})

(defn color-picker [{:keys [fx/context text item-key]}]
  {:fx/type :h-box
   :spacing 5
   :children [{:fx/type :color-picker
               :style-class "root-color-picker"
               :on-action {:event/type :color-change
                           :key item-key}}
              {:fx/type :label
               :style-class "root"
               :text text}]})

(defn color-selection-menu [{:keys [fx/context]}]
  {:fx/type :v-box
   :padding 7
   :spacing 20
   :children [{:fx/type color-picker
               :text "background-color"
               :item-key :background-color}
              {:fx/type color-picker
               :text "text-color"
               :item-key :text-color}
              {:fx/type color-picker
               :text "highlight-color"
               :item-key :highlight-color}
              {:fx/type color-picker
               :text "line-no-color"
               :item-key :line-no-color}
              {:fx/type color-picker
               :text "border-color"
               :item-key :border-color}
              {:fx/type color-picker
               :text "scroll-color"
               :item-key :scroll-color}
              {:fx/type color-picker
               :text "button-hover-color"
               :item-key :button-hover-color}
              {:fx/type button-layout
               :left-button {:fx/type new-button
                             :text "Cancel"
                             :event-type :close-theme-creator}
               :right-button {:fx/type new-button
                              :text "Confirm"
                              :event-type :style-change}}]})

(defn fake-scroll-bar [{:keys [fx/context]}]
  {:fx/type :rectangle
   :height 46
   :width 10
   :fill ((fx/sub-val context :colors) :scroll-color)})

(defn sample-status-row [{:keys [fx/context]}]
  {:fx/type :h-box
   :padding 14
   :children [{:fx/type :text-field
               :pref-height 32
               :pref-width 400
               :editable false
               :alignment :center
               :style (str "-fx-text-fill: " ((fx/sub-val context :colors) :text-color) "; "
                           "-fx-background-color: " ((fx/sub-val context :colors) :background-color) "; "
                           "-fx-highlight-fill: " ((fx/sub-val context :colors) :highlight-color) "; "
                           "-fx-border-color: " ((fx/sub-val context :colors) :border-color) "; "
                           "-fx-border-style: hidden hidden solid hidden; ")
               :text "/home/foo/bar/baz.txt"}]})

(defn sample-editor-row [{:keys [fx/context]}]
  {:fx/type :h-box
   :padding 22
   :children [{:fx/type :text-area
               :editable false
               :text "1\n2\n3\n4\n5\n6\n7\n"
               :pref-width 16
               :max-width 16
               :pref-height 230
               :font "Roboto Mono"
               :style (str "-fx-text-fill: " ((fx/sub-val context :colors) :line-no-color) "; "
                           "-fx-control-inner-background: " ((fx/sub-val context :colors) :background-color) "; "
                           "-fx-background-color: " ((fx/sub-val context :colors) :background-color) "; "
                           "-fx-border-color: " ((fx/sub-val context :colors) :background-color) "; "
                           "-fx-highlight-fill: " ((fx/sub-val context :colors) :highlight-color) "; ")}
              {:fx/type :text-area
               :editable false
               :pref-width 330
               :pref-height 230
               :font "Roboto Mono"
               :cursor :text
               :text "Put your test text here"
               :style (str "-fx-text-fill: " ((fx/sub-val context :colors) :text-color) "; "
                           "-fx-control-inner-background: " ((fx/sub-val context :colors) :background-color) "; "
                           "-fx-background-color: " ((fx/sub-val context :colors) :background-color) "; "
                           "-fx-border-color: " ((fx/sub-val context :colors) :background-color) "; "
                           "-fx-highlight-fill: " ((fx/sub-val context :colors) :highlight-color) "; ")}
              {:fx/type fake-scroll-bar}]})

(defn sample-editor [{:keys [fx/context]}]
  {:fx/type :v-box
   :style (str "-fx-background-color: " ((fx/sub-val context :colors) :background-color) "; ")
   :children [{:fx/type sample-status-row}
              {:fx/type sample-editor-row}
              {:fx/type :text-field
               :style-class "root-fsview-filename-input"
               :pref-height 25
               :pref-width 400
               :on-key-typed {:event/type :type-input
                              :key :theme-name-entered}}]})



(defn theme-creator [{:keys [fx/context]}]
  {:fx/type :h-box
   :children [{:fx/type sample-editor}
              {:fx/type color-selection-menu}]})

(defn themes-view [{:keys [fx/context]}]
  {:fx/type :list-view
   :style-class "root-fsview"
   :items (fx/sub-val context :themes)
   :on-mouse-clicked {:event/type :theme-pick}})