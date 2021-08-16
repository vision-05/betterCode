(ns bettercode.stages
  (:require [cljfx.api :as fx]
            [cljfx.css :as css]))

(defn main-app [{:keys [fx/context tclient]}]
  {:fx/type :stage
           :title "BetterCode"
           :showing true
           :width 768
           :height 1080
           :resizable true
           :scene {:fx/type :scene
                   :fill (((fx/sub-val context :colors) :background-color) 0)
                   :stylesheets [(::css/url (fx/sub-val context :style-sheet))]
                   :root {:fx/type :v-box
                          :children [{:fx/type :menu-bar
                                      :max-height 10 ;somehow make the height actually work
                                      :style-class "root-menu-bar"
                                      :menus [{:fx/type :menu
                                               :text "file"
                                               :style-class "root-menu-bar-item"
                                               :items [{:fx/type :menu-item
                                                        :style-class "root-menu-bar-item-sub-item"
                                                        :text "open"
                                                        :on-action {:event/type :open-file-explorer
                                                                    :tclient tclient}}
                                                       {:fx/type :menu-item
                                                        :style-class "root-menu-bar-item-sub-item"
                                                        :text "save"
                                                        :on-action {:event/type :save-file
                                                                    :tclient tclient}}
                                                       {:fx/type :menu-item
                                                        :style-class "root-menu-bar-item-sub-item"
                                                        :text "close file"
                                                        :on-action {:event/type :close-file
                                                                    :tclient tclient}}]}
                                              {:fx/type :menu
                                               :text "theme"
                                               :style-class "root-menu-bar-item"
                                               :items [{:fx/type :menu-item
                                                        :style-class "root-menu-bar-item-sub-item"
                                                        :text "new theme"
                                                        :on-action {:event/type :open-theme-creator}}
                                                       {:fx/type :menu-item
                                                        :style-class "root-menu-bar-item-sub-item"
                                                        :text "edit theme"}
                                                       {:fx/type :menu-item
                                                        :style-class "root-menu-bar-item-sub-item"
                                                        :text "existing theme"
                                                        :on-action {:event/type :open-theme-selector}}]}]}
                                     {:fx/type bettercode.elements/editor-pane
                                      :tclient tclient
                                      :text ""
                                      :file-path "BetterCode"
                                      :style-class "root"}]}}})

(defn file-explorer [{:keys [fx/context tclient]}]
  {:fx/type :stage
   :title (fx/sub-val context :cur-path)
   :showing (fx/sub-val context :file-explorer-show)
   :width 500
   :height 350
   :resizable false
   :always-on-top true
   :modality :application-modal
   :scene {:fx/type :scene
           :fill (((fx/sub-val context :colors) :background-color) 0)
           :stylesheets [(::css/url (fx/sub-val context :style-sheet))]
           :root {:fx/type bettercode.utilelements/file-window
                  :tclient tclient}}})

(defn theme-creator [{:keys [fx/context tclient]}]
  {:fx/type :stage
   :title "Theme"
   :showing (fx/sub-val context :theme-creator-show)
   :width 700
   :height 380
   :resizable false
   :scene {:fx/type :scene
           :fill (((fx/sub-val context :colors) :background-color) 0)
           :stylesheets [(::css/url (fx/sub-val context :style-sheet))]
           :root {:fx/type bettercode.utilelements/theme-creator}}})

(defn theme-picker [{:keys [fx/context tclient]}]
  {:fx/type :stage
   :title "Theme Picker"
   :showing (fx/sub-val context :theme-picker-show)
   :width 700
   :height 380
   :resizable false
   :scene {:fx/type :scene
           :fill (((fx/sub-val context :colors) :background-color) 0)
           :stylesheets [(::css/url (fx/sub-val context :style-sheet))]
           :root {:fx/type bettercode.utilelements/themes-view}}})

