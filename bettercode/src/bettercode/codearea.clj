(ns bettercode.codearea
  (:require [cljfx.api :as fx]
            [cljfx.fx.text-input-control]))

(gen-class
 :name bettercode.codearea.BetterCodeArea
 :extends javafx.scene.control.TextInputControl
 :prefix "bfx-"
 :init "init"
 :constructors {[] [javafx.scene.control.TextInputControl$Content]}
 :methods [["stateValue" [] java.lang.String]
           ["getAnchors" [] java.util.List]
           ["getCarets" [] java.util.List]])

(gen-class
 :name bettercode.codearea.BetterCodeArea.TextContent
 :implements [javafx.scene.control.TextInputControl$Content]
 :prefix "bfx-content-"
 :init "init"
 :constructors {[] []}
 :methods [["get" [Integer Integer] String]
           ["insert" [Integer String Boolean] Void]
           ["delete" [Integer Integer Boolean] Void]])

(defn bfx-init []
  [[(bettercode.codearea.BetterCodeArea.TextContent.)]])

(defn bfx-content-init []
  [[]])