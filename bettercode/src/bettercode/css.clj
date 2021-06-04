(ns bettercode.css
  (:require [cljfx.css :as css]
            [clojure.edn :as edn]))

(def colors (edn/read-string (slurp ".bettercode/conf.edn")))

(def style
  (css/register ::style
                (let [text-color (colors :text-color)
                      border-color (colors :border-color)
                      background-color (colors :background-color)
                      invisible "#00000000"
                      thumb-color (colors :scroll-color)
                      highlight-color (colors :highlight-color)
                      line-no-color (colors :line-no-color)
                      button-hover-color (colors :button-hover-color)]
                  {".root" {:-fx-background-color background-color
                            :-fx-text-fill text-color

                            "-color-picker" {:-fx-border-color border-color
                                             :-fx-border-radius 5
                                             :-fx-background-radius 5
                                             :-fx-control-inner-background background-color
                                             :-fx-background background-color

                                             " .color-picker-label" {:-fx-text-fill text-color}

                                             " .combo-box-popup" {:-fx-background-color background-color
                                                                  :-fx-background background-color
                                                                  :-fx-control-inner-background background-color}

                                             ":hover" {:-fx-background-color highlight-color}}

                            "-menu-bar" {:-fx-background-color background-color
                                         :-fx-control-inner-background background-color
                                         :-fx-text-fill text-color

                                         " .label" {:-fx-text-fill text-color
                                                    :-fx-font-size 12}

                                         "-item" {:-fx-background-color background-color
                                                  :-fx-control-inner-background background-color
                                                  :-fx-border-color border-color
                                                  :-fx-text-fill text-color

                                                  " .label" {:-fx-text-fill text-color
                                                             :-fx-font-size 10}

                                                  "-sub-item" {:-fx-background-color background-color
                                                               :-fx-control-inner-background background-color
                                                               :-fx-text-fill text-color
                                                               :-fx-padding [4 4 4 4]
                                                               :-fx-border-radius 5

                                                               ":hover" {:-fx-background-color button-hover-color
                                                                         :-fx-background-radius 5}}}}

                            "-fsview" {:-fx-control-inner-background background-color

                                       "-button" {:-fx-background-color background-color
                                                  :-fx-border-color border-color
                                                  :-fx-text-fill text-color
                                                  :-fx-padding [4 4 4 4]
                                                  :-fx-border-radius 5

                                                  ":hover" {:-fx-background-color button-hover-color
                                                            :-fx-background-radius 5}}

                                       "-filename-input" {:-fx-background-color background-color
                                                          :-fx-border-color border-color
                                                          :-fx-text-fill text-color
                                                          :-fx-padding [4 4 4 4]
                                                          :-fx-border-radius 5}

                                       "> .virtual-flow" {:-fx-background-color invisible
                                                          :-fx-hbar-policy :as-needed
                                                          :-fx-vbar-policy :as-needed

                                                          "> .corner" {:fx-background-color invisible}

                                                          "> .scroll-bar" {":horizontal" {:-fx-background-color invisible

                                                                                          " .thumb" {:-fx-background-color thumb-color}}

                                                                           ":vertical" {:-fx-background-color invisible

                                                                                        " .thumb" {:-fx-background-color thumb-color}}}}}

                            "-text-area" {"-status" {:-fx-background-color background-color
                                                     :-fx-border-color border-color
                                                     :-fx-border-style [:hidden :hidden :solid :hidden]
                                                     :-fx-text-fill text-color
                                                     :-fx-highlight-fill highlight-color}

                                          "-editor" {:-fx-background-color background-color
                                                     :-fx-text-fill text-color
                                                     :-fx-highlight-fill highlight-color

                                                     " .content" {:-fx-background-color background-color}

                                                     " .scroll-pane" {:-fx-background-color :transparent
                                                                      :-fx-hbar-policy :as-needed
                                                                      :-fx-vbar-policy :as-needed

                                                                      "> .corner" {:-fx-background-color invisible}

                                                                      " .scroll-bar" {" .decrement-button" {:-fx-opacity 0}

                                                                                      " .increment-button" {:-fx-opacity 0}

                                                                                      ":horizontal" {:-fx-background-color :transparent

                                                                                                     " .track" {:-fx-opacity 0}

                                                                                                     " .track-background" {:-fx-opacity 0}

                                                                                                     " .thumb" {:-fx-background-color thumb-color}}

                                                                                      ":vertical" {:-fx-background-color :transparent

                                                                                                   " .track" {:-fx-opacity 0}

                                                                                                   " .track-background" {:-fx-opacity 0}

                                                                                                   " .thumb" {:-fx-background-color thumb-color}}}}}

                                          "-numbers" {:-fx-background-color background-color
                                                      :-fx-text-fill line-no-color
                                                      :-fx-highlight-fill background-color

                                                      " *.text" {:-fx-text-alignment :right}

                                                      " .content" {:-fx-background-color background-color}

                                                      " .scroll-pane" {:-fx-hbar-policy :never
                                                                       :-fx-vbar-policy :never
                                                                       :-fx-background-color background-color}}}}})))

