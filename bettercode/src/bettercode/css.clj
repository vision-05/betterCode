(ns bettercode.css
  (:require [cljfx.css :as css]))

(def style
  (css/register ::style
                (let [back-background-color "#2E3440"
                      status-background-color "#434C5E"
                      text-color "#ECEFF4"
                      border-color "#4C566A"
                      background-color "#2E3440"
                      scroll-color "#434C5E22"
                      thumb-color "#4C566A44"
                      highlight-color "#4C566ACC"]
                  {".root" {:-fx-background-color back-background-color
                            "-fsview" {:-fx-control-inner-background back-background-color}
                            "-text-area" {"-status" {:-fx-background-color status-background-color
                                                     :-fx-text-fill text-color
                                                     :-fx-highlight-fill status-background-color}
                                          "-editor" {:-fx-text-fill text-color
                                                     :-fx-highlight-fill highlight-color
                                                     :-fx-background-color back-background-color
                                                     :-fx-border-color border-color
                                                     " .content" {:-fx-background-color background-color}
                                                     " .scroll-pane" {:-fx-background-color background-color
                                                                      :-fx-hbar-policy :as-needed
                                                                      :-fx-vbar-policy :as-needed
                                                                      "> .corner" {:-fx-background-color scroll-color}
                                                                      " .scroll-bar" {":horizontal" {:-fx-background-color scroll-color
                                                                                                     " .thumb" {:-fx-background-color thumb-color}}
                                                                                      ":vertical" {:-fx-background-color scroll-color
                                                                                                   " .thumb" {:-fx-background-color thumb-color}}}}}
                                          "-numbers" {:-fx-text-fill text-color
                                                      :-fx-background-color background-color
                                                      :-fx-border-color border-color
                                                      " .content" {:-fx-background-color background-color}
                                                      " .scroll-pane" {:-fx-hbar-policy :never
                                                                       :-fx-vbar-policy :never
                                                                       :-fx-background-color background-color
                                                                       :-fx-padding 10
                                                                       :-fx-border-insets 10
                                                                       :-f-background-insets 10}}}}})))

