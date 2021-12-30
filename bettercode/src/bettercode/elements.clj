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

(def create-codearea (constantly (CodeArea.)))

(defn text-edit [{:keys [fx/context tclient]}] ;these subscriptions replace entire component, not for textarea
  {:fx/type code-area/with-richtext-props
   :desc {:fx/type fx/ext-instance-factory
          :create create-codearea}
          :props {:text (fx/sub-val context :text-editor)
                  :line-number false
                  :style-spans (fx/sub-val context :spans)
                  :style-class "root-text-area-editor"
                  :on-key-pressed {:event/type :key-press ;figure out this event thing
                                   :tclient tclient}
                  :on-mouse-clicked {:event/type :text-click}}})

(defn status-row [{:keys [fx/context]}]
  {:fx/type :h-box
   :pref-width 768
   :padding 14
   :children [{:fx/type status-bar
               :file-path (fx/sub-val context :file-path)
               :h-box/hgrow :always}]})

(defn editor-row [{:keys [fx/context tclient]}]
  {:fx/type :h-box
   :pref-width 5000
   :pref-height 5000
   :padding 14
   :spacing 28
   :children [{:fx/type text-edit ;this doesn't expand downwards for some reason
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
