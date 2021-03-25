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
                            "-text-area" {"-status" {:-fx-background-color status-background-color
                                                     :-fx-text-fill text-color
                                                     :-fx-highlight-fill status-background-color}
                                          "-editor" {:-rtfx-background-color background-color}
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

