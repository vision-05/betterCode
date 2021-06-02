(ns bettercode.css
  (:require [cljfx.css :as css]))

(def style
  (css/register ::style
                (let [back-background-color "#2E3440"
                      status-background-color "#4C566A"
                      text-color "#ECEFF4"
                      border-color "#4C566A"
                      background-color "#2E3440"
                      scroll-color "#434C5E00"
                      thumb-color "#4C566A22"
                      highlight-color "#4C566A"
                      line-no-color "#4C566A"]
                  {".root" {:-fx-background-color back-background-color
                            :-fx-text-fill text-color
                            "-menu-bar" {:-fx-background-color highlight-color
                                         :-fx-text-fill text-color
                                         :-fx-control-inner-background highlight-color
                                         " .label" {:-fx-font-size 12
                                                    :-fx-text-fill text-color}
                                         "-item" {:-fx-text-fill text-color
                                                  :-fx-background-color highlight-color
                                                  :-fx-control-inner-background highlight-color
                                                  :-fx-border-color highlight-color
                                                  " .label" {:-fx-font-size 10
                                                             :-fx-text-fill text-color}
                                                  "-sub-item" {:-fx-text-fill text-color
                                                               :-fx-background-color highlight-color
                                                               :-fx-control-inner-background highlight-color
                                                               :-fx-padding [4 4 4 4]
                                                               ":hover" {:-fx-background-color background-color
                                                                         :-fx-background-radius 5}}}}
                            "-fsview" {:-fx-control-inner-background back-background-color
                                       "-button" {:-fx-background-color back-background-color
                                                  :-fx-border-color border-color
                                                  :-fx-text-fill text-color
                                                  :-fx-border-radius 5
                                                  :-fx-padding [4 4 4 4]
                                                  ":hover" {:-fx-background-color border-color
                                                            :-fx-background-radius 5}}
                                       "-filename-input" {:-fx-background-color back-background-color
                                                          :-fx-border-color border-color
                                                          :-fx-text-fill text-color
                                                          :-fx-padding [4 4 4 4]
                                                          :-fx-border-radius 5}
                                       "> .virtual-flow" {:-fx-background-color scroll-color
                                                          :-fx-hbar-policy :as-needed
                                                          :-fx-vbar-policy :as-needed
                                                          "> .corner" {:fx-background-color scroll-color}
                                                          "> .scroll-bar" {":horizontal" {:-fx-background-color scroll-color
                                                                                          " .thumb" {:-fx-background-color thumb-color}}
                                                                           ":vertical" {:-fx-background-color scroll-color
                                                                                        " .thumb" {:-fx-background-color thumb-color}}}}}
                            "-text-area" {"-status" {:-fx-background-color back-background-color
                                                     :-fx-border-color border-color
                                                     :-fx-border-style [:hidden :hidden :solid :hidden]
                                                     :-fx-text-fill text-color
                                                     :-fx-highlight-fill status-background-color}
                                          "-editor" {:-fx-text-fill text-color
                                                     :-fx-highlight-fill highlight-color
                                                     :-fx-background-color back-background-color
                                                     ;:-fx-border-color border-color
                                                     " .content" {:-fx-background-color background-color}
                                                     " .scroll-pane" {:-fx-background-color :transparent
                                                                      :-fx-hbar-policy :as-needed
                                                                      :-fx-vbar-policy :as-needed
                                                                      "> .corner" {:-fx-background-color scroll-color}
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
                                          "-numbers" {:-fx-text-fill line-no-color
                                                      :-fx-background-color background-color
                                                      ;:-fx-border-color border-color
                                                      :-fx-highlight-fill background-color
                                                      " *.text" {:-fx-text-alignment :right}
                                                      " .content" {:-fx-background-color background-color}
                                                      " .scroll-pane" {:-fx-hbar-policy :never
                                                                       :-fx-vbar-policy :never
                                                                       :-fx-background-color background-color}}}}})))

