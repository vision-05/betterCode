(ns bettercode.elements
  (:require [cljfx.api :as fx]
            [bettercode.css]
            [bettercode.events]))

(defn status-bar [{:keys [fx/context file-path]}]
  {:fx/type :text-field
   :min-height 32
   :max-height 32
   :min-width 740
   :editable false
   :alignment :center
   :text file-path
   :style-class "root-text-area-status"})

(defn text-edit [{:keys [fx/context tclient]}]
  {:fx/type :text-area
   :pref-width 632
   :pref-height 896
   :font "Roboto Mono"
   :cursor :text
   :text (fx/sub context :text-editor)
   :on-key-typed {:event/type :bettercode.events/text-type}
   :on-scroll {:event/type :bettercode.events/scroll}
   :style-class "root-text-area-editor"})

(defn line-numbers [{:keys [fx/context]}]
  {:fx/type :text-area
   :editable false
   :text (fx/sub context :line-numbers)
   :pref-width 40
   :pref-height 896
   :scroll-top (fx/sub context :vscroll)
   :font "Roboto Mono"
   :style-class "root-text-area-numbers"})

(defn status-row [{:keys [fx/context]}]
  {:fx/type :h-box
   :min-width 768
   :padding 14
   :children [{:fx/type status-bar
               :file-path (fx/sub context :file-path)
               :h-box/hgrow :always}]})

(defn editor-row [{:keys [fx/context tclient]}]
  {:fx/type :h-box
   :pref-width 768
   :min-width 768
   :padding 14
   :spacing 28
   :children [{:fx/type line-numbers
               :h-box/vgrow :always}
              {:fx/type text-edit ;this doesn't expand downwards for some reason
               :tclient tclient
               :h-box/vgrow :always
               :h-box/hgrow :always}]})

(defn editor-pane [{:keys [fx/context tclient]}]
  {:fx/type :v-box
   :pref-width 768
   :min-width 768
   :min-height 1080
   :children [{:fx/type status-row
               :v-box/hgrow :always}
              {:fx/type editor-row
               :tclient tclient
               :v-box/vgrow :always
               :v-box/hgrow :always}]})

(defn fs-view [{:keys [fx/context tclient]}]
  {:fx/type :list-view})