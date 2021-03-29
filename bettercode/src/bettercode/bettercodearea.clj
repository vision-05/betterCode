(ns bettercode.bettercodearea
  (:require
   [cljfx.composite :as composite]
   [cljfx.lifecycle :as lifecycle]
   [cljfx.coerce :as coerce]
   [cljfx.fx.text-input-control])
  (:import
   [javafx.geometry Pos]
   [javafx.scene AccessibleRole]
   [bettercode.codearea BetterCodeArea]))

(def props
  (merge
   cljfx.fx.text-input-control/props
   (composite/props BetterCodeArea
                    :style-class [:list lifecycle/scalar :coerce coerce/style-class
                                  :default ["text-input"]]
                    :accessible-role [:setter lifecycle/scalar :coerce (coerce/enum AccessibleRole)
                                      :default :text-area]
                    :on-action [:setter lifecycle/event-handler :coerce coerce/event-handler])))

(def lifecycle
  (composite/describe BetterCodeArea
                      :ctor []
                      :props ::props))