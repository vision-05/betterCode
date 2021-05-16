(ns bettercode.utilelements
  (:require [cljfx.api :as fx]
            [manifold.stream :as s]
            [bettercode.events]))

(defn file-window [{:keys [fx/context tclient]}]
    {:fx/type :list-view
     :items (fx/sub context :dir-contents)
     :style-class "root-fsview"
     :on-mouse-clicked {:event/type :bettercode.events/fexclick
                        :fx/sync true
                        :tclient tclient}})