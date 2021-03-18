(ns bettercode.css
  (:require [cljfx.css :as css]))

(def style
  (css/register ::style
                (let [back-background-color "#23282D"
                      status-background-color "#275F77"
                      text-color "#E0CDCD"
                      background-color "#123F58"]
                  {".root" {:-fx-background-color back-background-color
                            "-text-area" {"-status" {:-fx-background-color status-background-color
                                                     :-fx-text-fill text-color}
                                          "-editor" {:-fx-text-fill text-color
                                                     :-fx-background-color back-background-color
                                                     " .content" {:-fx-background-color background-color}
                                                     " .scroll-pane" {:-fx-hbar-policy :never
                                                                      :-fx-vbar-policy :never
                                                                      :-fx-background-color background-color
                                                                      :-fx-padding 10
                                                                      :-fx-border-insets 10
                                                                      :-f-background-insets 10}}
                                          "-numbers" {:-fx-text-fill text-color
                                                      :-fx-background-color back-background-color
                                                      " .content" {:-fx-background-color status-background-color}
                                                      " .scroll-pane" {:-fx-hbar-policy :never
                                                                       :-fx-vbar-policy :never
                                                                       :-fx-background-color status-background-color
                                                                       :-fx-padding 10
                                                                       :-fx-border-insets 10
                                                                       :-f-background-insets 10}}}}})))

