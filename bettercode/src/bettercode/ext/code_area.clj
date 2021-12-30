(ns bettercode.ext.code-area
  (:require [cljfx.lifecycle :as lifecycle]
            [cljfx.prop :as prop]
            [cljfx.mutator :as mutator]
            [cljfx.composite :as composite]
            [cljfx.coerce :as coerce])
  (:import [javafx.util Callback]
           [org.fxmisc.richtext CodeArea]
           [org.fxmisc.richtext LineNumberFactory]))

(set! *warn-on-reflection* true)

(defn- coerce-text [x]
  (cond (string? x) x
        (vector? x) x
        :else (coerce/fail `coerce-text x)))

(defn- coerce-span [spans]
  (cond (and (vector? spans) (= '() (filter false? (for [span spans
                                                         :let [ks (keys span)]]
                                                     (= ks '(:start :end :style)))))) spans
        :else (coerce/fail `coerce-span spans)))

(def with-richtext-props
  "Extension lifecycle for a richtext area. Supported keys: (maybe add more later)
  - `:text`, the text content of the area
  - `:read-only`, the read only property of the area
  - `:wrap-text`, the wrap property of the area
  - `:line-number`, the line number property of the area
  - `:style-class`, the starting style of the area
  - `:style-spans`, the vector of style spans for the area
  - `:on-text-changed`, the event handler map for text change
  - `:on-key-pressed`, the event handler map for a keypress
  - `:on-scroll`, the event handler map for a scroll this might be useless as line numbers included in this text area
  - `:on-mouse-clicked` the event handler map for a mouse click
  - `:pref-height` the vgrow policy
  - `:pref-width` the hgrow policy"
  (lifecycle/make-ext-with-props lifecycle/dynamic
                                 {:text (prop/make
                                         (mutator/setter
                                          #(when-not (= (.getText %1) %2)
                                             (prn "text: " %2)
                                             (.replace %1 0 0 %2 "")))
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
                                  :line-number (prop/make
                                                (mutator/setter
                                                 #(when (= %2 true)
                                                    (.setParagraphGraphicFactory %1 (LineNumberFactory/get %1))))
                                                lifecycle/scalar)
                                  :style-class (prop/make
                                                (mutator/observable-list
                                                 (composite/observable-list CodeArea :style-class))
                                                lifecycle/scalar
                                                :coerce coerce/style-class)
                                  :style-spans (prop/make
                                                (mutator/setter #(doseq [style %2]
                                                                   (println "highlight: " %1 (.getLength %1) (style :end) %2)
                                                                   (when-not (< (.getLength %1) (style :end))
                                                                     (.setStyleClass %1 (style :start) (style :end) (style :style))))) ;I have to do a CSS file for this
                                                lifecycle/scalar
                                                :coerce coerce-span)
                                  :on-text-changed (prop/make
                                                    (mutator/property-change-listener
                                                     #(do
                                                        (prn "   " %1 "%%%")
                                                        (.textProperty %1 %2)))
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
