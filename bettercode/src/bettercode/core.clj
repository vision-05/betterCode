(ns bettercode.core
  (:gen-class)
  (:require [cljfx.api :as fx]
            [cljfx.css :as css]
            [clojure.string :as str]
            [clojure.data :as data]
            [clojure.core.cache :as cache]))

(def style
  (css/register ::style
    (let [padding 20
          text-color "#E0CDCD"
          editor-color "#123F58"
          status-color "#275F77"]
      {".text" {:-fx-text-fill text-color
                "-editor" {:-fx-control-inner-background editor-color}
                "-status" {:-fx-background-color status-color}}})))

(def *context
  (atom 
    (fx/create-context {:title "BetterCode"
                        :file-name "TextEditor.cpp"
                        :text-editor (slurp "/home/tim/betterCode/Server/TextEditor.cpp")
                        :action ""
                        :argument "hehe"}
                       #(cache/lru-cache-factory % :threshold 4096))))

(defn status-bar [{:keys [fx/context variant]}]
  "A status bar that stores the current column, line and filename"
  {:fx/type :label
   :text (fx/sub-val context :file-name)
   :style (str "text-" variant)})

(defn text-edit [{:keys [fx/context variant]}]
  "A text editing area that updates the text-editor context"
  {:fx/type :text-area
   :text (fx/sub-val context :text-editor)
   :style-class (str "text-" variant)
   :on-text-changed {:event/type ::type-text :fx/sync true}})

(defn line-numbers [{:keys [fx/context variant]}]
  {:fx/type :text-area
   :style-class (str "text-" variant)})

(defn editor-line-numbers [{:keys [fx/context]}]
  "An editor with line numbers in an hbox layout"
  {:fx/type :h-box
    :padding 0
    :spacing 20
    :children [{:fx/type line-numbers
                :variant "editor"}
               {:fx/type text-edit
                :variant "editor"
                :h-box/hgrow :always}]})

(defn editor-pane [{:keys [fx/context]}]
  "The whole editor pane, made up of a status bar, line number bar and actual editor"
  {:fx/type :v-box
   :alignment :top-center
   :padding 20
   :spacing 20
   :children [{:fx/type status-bar
               :variant "status"}
              {:fx/type editor-line-numbers
               :v-box/vgrow :always}]})

(defmulti handle-event :event/type)

(defmethod handle-event :default [e]
  (println e))

(defmethod handle-event ::type-text [{:keys [fx/event fx/context]}]
  {:context (fx/swap-context context assoc :text-editor event 
                                           :argument (data/diff event (fx/sub-val context :text-editor))
                                           :action "text-edit")})

(defn -main []
  (fx/create-app *context
    :event-handler handle-event
    :desc-fn (fn [_]
                {:fx/type :stage
                 :showing true
                 :width 768
                 :height 1080
                 :scene {:fx/type :scene
                         :stylesheets [(::css/url style)]
                         :root {:fx/type editor-pane}}})))