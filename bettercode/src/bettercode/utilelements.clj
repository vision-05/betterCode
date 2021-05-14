(ns bettercode.utilelements
  (:require [cljfx.api :as fx]
            [manifold.stream :as s]
            [bettercode.events]))

(defn file-window [{:keys [fx/context]}]
  {:fx/type :v-box})