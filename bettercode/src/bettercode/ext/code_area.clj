(ns bettercode.ext.code-area
  (:require [cljfx.lifecycle :as lifecycle]
            [cljfx.prop :as prop]
            [cljfx.mutator :as mutator]
            [cljfx.coerce :as coerce])
  (:import [javafx.util Callback]
           [org.fxmisc.richtext CodeArea]))

(set! *warn-on-reflection* true)

(defn- coerce-text [x]
  (cond (string? x) x
        :else (coerce/fail `coerce-text x)))

(def with-richtext-props
  "Extension lifecycle for a richtext area. Supported keys: (maybe add more later)
  - `:text`, the text content of the area
  - `:read-only`, the read only property of the area
  - `:wrap-text`, the wrap property of the area
  - `:style-sheet`, the starting style of the area
  - `:style-spans`, the vector of style spans for the area
  - `:on-text-changed`, the event handler map for text change
  - `:on-key-pressed`, the event handler map for a keypress
  - `:on-scroll`, the event handler map for a scroll this might be useless as line numbers included in this text area
  - `:on-mouse-clicked` the event handler map for a mouse click"
  (lifecycle/make-ext-with-props lifecycle/dynamic
                                 {:text (prop/make
                                         (mutator/setter
                                          #(.replace %1 0 0 %2 ""))
                                         lifecycle/scalar
                                         :coerce coerce-text)
                                  :read-only (prop/make
                                              (mutator/setter
                                               #(.setEditable %1 %2))
                                              lifecycle/scalar)
                                  :wrap-text (prop/make
                                              (mutator/setter
                                               #(.setWrapText %1 %2))
                                              lifecycle/scalar)
                                  :style-sheet (prop/make
                                                (mutator/setter
                                                 #(.setStyleClass %1 0 0 %2))
                                                lifecycle/scalar)
                                  :on-text-changed (prop/make
                                                    (mutator/property-change-listener
                                                     #(.textProperty %1 %2))
                                                    lifecycle/change-listener)
                                  :on-key-pressed (prop/make
                                                   (mutator/setter
                                                    #(.setOnKeyPressed %1 %2))
                                                   lifecycle/event-handler
                                                   :coerce coerce/event-handler)
                                  :on-scroll (prop/make
                                              (mutator/setter
                                               #(.setOnScroll %1 %2))
                                              lifecycle/event-handler
                                              :coerce coerce/event-handler)
                                  :on-mouse-clicked (prop/make
                                                     (mutator/setter
                                                      #(.setOnMouseClicked %1 %2))
                                                     lifecycle/event-handler
                                                     :coerce coerce/event-handler)}))
