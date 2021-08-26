(ns bettercode.css
  (:gen-class)
  (:require [cljfx.css :as css]
            [clojure.edn :as edn]
            [bettercode.meta]))

(def colors (edn/read-string (slurp (bettercode.meta/conf-info :theme-path))))

(defn background-radius
  "function that returns a map that sets background radius"
  [radius]
  {:-fx-background-radius radius})

(defn background
  "function that returns a map that either sets background colour, or background colour and radius"
  ([[color _]] {:-fx-background-color color})
  ([color radius] (conj (background color) (background-radius radius))))

(defn inner-background
  "function that returns a map that sets inner background colour"
  ([[color _]] {:-fx-control-inner-background color})
  ([color _] (conj (inner-background color) {:-fx-background color})))

(defn back-and-inner
  "function that returns a map that sets the background and inner background colour"
  [color]
  (conj (background color) (inner-background color "")))

(defn text
  "function that returns a map that either sets text color or text color and text size"
  ([[color _]] {:-fx-text-fill color})
  ([color size] (conj (text color) {:-fx-font-size size})))

(defn border-radius
  "function that returns a map that sets border radius"
  [radius]
  {:-fx-border-radius radius})

(defn border
  "function that returns a map that either sets border colour or border colour and border radius"
  ([[color _]] {:-fx-border-color color})
  ([color radius] (conj (border color) (border-radius radius))))

(defn highlight
  "function that returns a map that sets highlight colour"
  [[color _]]
  {:-fx-highlight-fill color})

(defn root
  "function that returns the root CSS map"
  [{:keys [background-color text-color]}]
  (conj (background background-color) (text text-color)))

(defn color-picker
  "function which returns the color-picker CSS map"
  [{:keys [background-color border-color text-color highlight-color]}]
  (conj (background background-color 5)
        (border border-color 5)
        (text text-color)
        (background-radius 5)
        {":hover" (background highlight-color)}))

(defn color-picker-label
  "function which returns the color-picker-label CSS map"
  [{:keys [text-color]}]
  (text text-color))

(defn color-picker-combo-popup
  "function which returns the color-picker-combo-popup CSS map"
  [{:keys [background-color]}]
  (back-and-inner background-color))

(defn menu-bar
  "function which returns the menu-bar CSS map"
  [{:keys [background-color text-color]}]
  (conj (back-and-inner background-color)
        (text text-color)))

(defn menu-bar-label
  "function which returns the menu-bar-label CSS map"
  [{:keys [text-color]}]
  (text text-color 12))

(defn menu-bar-item
  "function which returns the menu-bar-item CSS map"
  [{:keys [background-color text-color border-color]}]
  (conj (back-and-inner background-color)
        (border border-color)
        (text text-color)))

(defn menu-bar-item-label [{:keys [text-color]}]
  (text text-color 10))

(defn menu-bar-sub-item [{:keys [background-color text-color button-hover-color]}]
  (conj (back-and-inner background-color)
        (border background-color 5)
        (text text-color)
        {:-fx-padding [4 4 4 4]
         ":hover" (background button-hover-color 5)}))

(defn fsview [{:keys [background-color text-color]}]
  (conj (inner-background background-color)
        (text text-color)))

(defn fsview-button [{:keys [background-color border-color text-color button-hover-color]}]
  (conj (background background-color)
        (border border-color 5)
        (text text-color)
        {:-fx-padding [4 4 4 4]
         ":hover" (background button-hover-color 5)}))

(defn fsview-filename-input [{:keys [background-color border-color text-color]}]
  (conj (background background-color)
        (border border-color 5)
        (text text-color)
        {:-fx-padding [4 4 4 4]}))

(defn fsview-vflow [_]
  {:-fx-background-color ["#00000000"]
   :-fx-hbar-policy :as-needed
   :-fx-vbar-policy :as-needed})

(defn fsview-vflow-corner [_]
  (background ["#00000000"]))

(defn fsview-vflow-scroll-bar [{:keys [scroll-color]}]
  {":horizontal" (conj (background ["#00000000"])
                       {" .thumb" (background scroll-color)})
   ":vertical" (conj (background ["#00000000"])
                     {" .thumb" (background scroll-color)})})

(defn text-area-status [{:keys [background-color border-color text-color highlight-color]}]
  (conj (background background-color)
        (border border-color)
        (text text-color)
        (highlight highlight-color)
        {:-fx-border-style [:hidden :hidden :solid :hidden]}))

(defn text-area-editor [{:keys [background-color text-color highlight-color]}]
  (conj (background background-color)
        (text text-color)
        (highlight highlight-color)))

(defn text-area-editor-content [{:keys [background-color]}]
  (background background-color))

(defn text-area-editor-scroll-pane [_]
  (conj (background ["#00000000"])
        {:-fx-hbar-policy :as-needed
         :-fx-vbar-policy :as-needed}))

(defn text-area-editor-scroll-pane-corner [_]
  (background ["#00000000"]))

(defn text-area-editor-scroll-pane-scroll-bar [{:keys [scroll-color]}]
  {" .decrement-button" {:-fx-opacity 0}
   " .increment-button" {:-fx-opacity 0}
   ":horizontal" (conj (background ["#00000000"])
                       {" .track" {:-fx-opacity 0}
                        " .track-background" {:-fx-opacity 0}
                        " .thumb" (background scroll-color)})
   ":vertical" (conj (background ["#00000000"])
                     {" .track" {:-fx-opacity 0}
                      " .track-background" {:-fx-opacity 0}
                      " .thumb" (background scroll-color)})})

(defn text-colors [[colour weight]]
  {:-fx-fill colour
   :-fx-font-weight (keyword weight)})

(defn make-text-colors [color-map]
  (reduce-kv #(assoc %1 (str "." (symbol %2)) (text-colors %3)) {} color-map))

(defn compose-style-map
  "make a style map"
  [color-map]
  (assoc (make-text-colors color-map)
         ".root"
         (-> (root color-map)
             (assoc "-color-picker"
                    (assoc (color-picker color-map)
                           " .color-picker-label"
                           (color-picker-label color-map)
                           " .combo-box-popup"
                           (color-picker-combo-popup color-map)))
             (assoc "-menu-bar"
                    (assoc (menu-bar color-map)
                           " .label"
                           (menu-bar-label color-map)
                           "-item"
                           (assoc (menu-bar-item color-map)
                                  " .label"
                                  (menu-bar-item-label color-map)
                                  "-sub-item"
                                  (menu-bar-sub-item color-map))))
             (assoc "-fsview"
                    (assoc (fsview color-map)
                           "-button"
                           (fsview-button color-map)
                           "-filename-input"
                           (fsview-filename-input color-map)
                           "> .virtual-flow"
                           (assoc (fsview-vflow color-map)
                                  "> .corner"
                                  (fsview-vflow-corner color-map)
                                  "> .scroll-bar"
                                  (fsview-vflow-scroll-bar color-map))))
             (assoc "-text-area"
                    (assoc {}
                           "-status"
                           (text-area-status color-map)
                           "-editor"
                           (assoc (text-area-editor color-map)
                                  " .content"
                                  (text-area-editor-content color-map)
                                  " .scroll-pane"
                                  (assoc (text-area-editor-scroll-pane color-map)
                                         "> .corner"
                                         (text-area-editor-scroll-pane-corner color-map)
                                         " .scroll-bar"
                                         (text-area-editor-scroll-pane-scroll-bar color-map))))))))

(defn make-style-sheet [color-map]
  (css/register ::style (compose-style-map color-map)))

(def style
  (make-style-sheet colors))
