(ns bettercode.utilelements
  (:require [cljfx.api :as fx]
            [manifold.stream :as s]
            [bettercode.events]))

(defn back-button [{:keys [fx/context tclient]}]
  {:fx/type :button
   :style-class "root-fsview-button"
   :on-mouse-clicked {:event/type :bettercode.events/backclick}
   :text "Back"})

(defn new-button [{:keys [fx/context tclient]}]
  {:fx/type :button
   :style-class "root-fsview-button"
   :on-mouse-clicked {:event/type :bettercode.events/newclick
                      :fx/sync true
                      :tclient tclient}
   :text "New"})

(defn button-layout [{:keys [fx/context tclient]}]
  {:fx/type :h-box
   :alignment :center-right
   :padding 5
   :spacing 10
   :children [{:fx/type back-button
               :tclient tclient}
              {:fx/type new-button
               :tclient tclient}]})

(defn file-view [{:keys [fx/context tclient]}]
    {:fx/type :list-view
     :items (fx/sub context :dir-contents)
     :style-class "root-fsview"
     :on-mouse-clicked {:event/type :bettercode.events/fexclick
                        :fx/sync true
                        :tclient tclient}})

(defn file-window [{:keys [fx/context tclient]}]
  {:fx/type :v-box
   :children [{:fx/type file-view
               :tclient tclient}
              {:fx/type button-layout
               :tclient tclient}]})