(ns bettercode.elements
  (:require [cljfx.api :as fx]
            [cljfx.css :as css]
            [clojure.data :as data]))

(def style
  (css/register ::style
                (let [back-background-color "#23282D"
                      status-background-color "#275F77"
                      text-color "#E0CDCD"
                      background-color "#123F58"]
                  {".text-area" {"-status" {:-fx-background-color status-background-color
                                            :-fx-text-fill text-color}
                                 "-editor" {:-fx-text-fill text-color
                                            :-fx-background-color back-background-color
                                            " .content" {:-fx-background-color background-color}
                                            " .scroll-pane" {:-fx-hbar-policy :never
                                                             :-fx-vbar-policy :never}}
                                 "-numbers" {:-fx-text-fill text-color
                                             :-fx-background-color back-background-color
                                             " .content" {:-fx-background-color status-background-color}
                                             " .scroll-pane" {:-fx-hbar-policy :never 
                                                              :-fx-vbar-policy :never}}}})))

(defmulti handle-event :event/type)

(defmethod handle-event :default [e]
  (println e))

(defmethod handle-event ::type-text [{:keys [fx/event fx/context]}]
  (println (slurp (::css/url bettercode.elements/style)))
  {:context (fx/swap-context context
                             assoc
                             :text-editor event
                             :argument (data/diff event (fx/sub-val context :text-editor))
                             :action "text-edit")})
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
   :style-class "text-area-status"})

(defn text-edit [{:keys [fx/context]}] ;put in scroll pane and hide/disable scroll bars
  {:fx/type :text-area
   :pref-width 632
   :pref-height 896
   :max-width 632
   :max-height 896
   :wrap-text true
   :text (fx/sub-val context :text-editor)
   :style-class "text-area-editor"
   :on-text-changed {:event/type ::type-text :fx/sync true}})

(defn line-numbers [{:keys [fx/context]}]
  {:fx/type :text-area
   :editable false
   :pref-width 80
   :pref-height 896
   :max-width 80
   :max-height 896
   :style-class "text-area-numbers"})

(defn status-row [{:keys [fx/context]}]
  {:fx/type :h-box
   :pref-width 768
   :children [{:fx/type spacer
               :width 14
               :height 32}
              {:fx/type status-bar}
              {:fx/type spacer
               :width 14
               :height 32
               :h-box/hgrow :always}]})

(defn editor-row [{:keys [fx/context]}]
  {:fx/type :h-box
   :pref-width 768
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
               :height 896
               :h-box/hgrow :always}]})

(defn editor-pane [{:keys [fx/context]}]
  {:fx/type :v-box
   :pref-width 768
   :pref-height 1080
   :children [{:fx/type spacer
               :height 30
               :width 768}
              {:fx/type status-row}
              {:fx/type spacer
               :height 26
               :width 768
               :v-box/vgrow :always}
              {:fx/type editor-row}
              {:fx/type spacer
               :height 96
               :width 768}]})