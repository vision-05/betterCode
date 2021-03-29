(ns bettercode.codearea
  (:require [cljfx.api :as fx]
            [cljfx.fx.text-input-control])
  (:import [javafx.scene.control TextInputControl]))
  
(gen-class
   :name bettercode.codearea.BetterCodeArea
   :extends javafx.scene.control.TextInputControl
   :state "state"
   :init "init"
   :prefix "bfx-"
   :constructors {[java.lang.String java.lang.String] [java.lang.String java.lang.String]
                  [] []}
   :methods [["stateValue" [] java.lang.String]
             ["getAnchors" [] java.util.List]
             ["getCarets" [] java.util.List]])

(defn bfx-init
  ([]
   [[]])
  ([text style]
   [[] (atom text) (atom style)]))

(defn bfx-stateValue [this]
  @(.state this))