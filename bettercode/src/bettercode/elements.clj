(ns bettercode.elements
  (:require [cljfx.api :as fx]
            [bettercode.css]
            [bettercode.events]
            [bettercode.ext.code-area :as code-area])
  (:import [org.fxmisc.richtext CodeArea]))

(defn status-bar [{:keys [fx/context file-path]}]
  {:fx/type :text-field
   :min-height 32
   :max-height 32
   :editable false
   :alignment :center
   :text file-path
   :style-class "root-text-area-status"})

(defn text-edit [{:keys [fx/context tclient]}]
  {:fx/type code-area/with-richtext-props
   :desc {:fx/type fx/ext-instance-factory
          :create #(CodeArea.)}
   :props {
   :text (fx/sub-val context :text-editor)
   :on-key-pressed {:event/type :key-press
                    :tclient tclient}
   :on-scroll {:event/type :scroll}
   :on-mouse-clicked {:event/type :text-click} }
                                        ;:style-class "root-text-area-editor"
   })

(defn line-numbers [{:keys [fx/context]}]
  {:fx/type :text-area
   :editable false
   :text (fx/sub-val context :line-numbers)
   :pref-width 40
   :pref-height 896
   :scroll-top (fx/sub-val context :vscroll)
   :font "Roboto Mono"
   :style-class "root-text-area-numbers"})

(defn status-row [{:keys [fx/context]}]
  {:fx/type :h-box
   :pref-width 768
   :padding 14
   :children [{:fx/type status-bar
               :file-path (fx/sub-val context :file-path)
               :h-box/hgrow :always}]})

(defn editor-row [{:keys [fx/context tclient]}]
  {:fx/type :h-box
   :pref-width 768
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
   :children [{:fx/type status-row
               :v-box/hgrow :always}
              {:fx/type editor-row
               :tclient tclient
               :v-box/vgrow :always
               :v-box/hgrow :always}]})

(defn fs-view [{:keys [fx/context tclient]}]
  {:fx/type :list-view})
