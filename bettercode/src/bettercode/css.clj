(ns bettercode.css
  (:require [cljfx.css :as css]
            [clojure.edn :as edn]
            [bettercode.meta]))

(def colors (edn/read-string (slurp (bettercode.meta/conf-info :theme-path))))

(defn background-radius [radius]
  {:-fx-background-radius radius})
(defn background
  ([color] {:-fx-background-color color})
  ([color radius] (conj (background color) (background-radius radius))))

(defn inner-background
  ([color] {:-fx-control-inner-background color})
  ([color _] (conj (inner-background color) {:-fx-background color})))

(defn back-and-inner [color]
  (conj (background color) (inner-background color "")))

(defn text
  ([color] {:-fx-text-fill color})
  ([color size] (conj (text color) {:-fx-font-size size})))

(defn border-radius [radius]
  {:-fx-border-radius radius})

(defn border
  ([color] {:-fx-border-color color})
  ([color radius] (conj (border color) (border-radius radius))))

(defn highlight [color]
  {:-fx-highlight-fill color})

(defn root [{:keys [background-color text-color]}]
  {".root" (conj (background background-color) (text text-color))})

(defn color-picker [{:keys [background-color border-color text-color highlight-color]}]
  (conj (background background-color 5)
        (border border-color 5)
        (text text-color)
        (background-radius 5)
        {":hover" (background highlight-color)}))

(defn color-picker-label [{:keys [text-color]}]
  (text text-color))

(defn color-picker-combo-popup [{:keys [background-color]}]
  (back-and-inner background-color))

(defn menu-bar [{:keys [background-color text-color]}]
  (conj (back-and-inner background-color)
                     (text text-color)))

(defn menu-bar-label [{:keys [text-color]}]
  (text text-color 12))

(defn menu-bar-item [{:keys [background-color text-color border-color]}]
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
  {:-fx-background-color "#00000000"
   :-fx-hbar-policy :as-needed
   :-fx-vbar-policy :as-needed})

(defn fsview-vflow-corner [_]
  (background "#00000000"))

(defn fsview-vflow-scroll-bar [{:keys [scroll-color]}]
  {":horizontal" (conj (background "#00000000")
                       {" .thumb" (background scroll-color)})
   ":vertical" (conj (background "#00000000")
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
  (conj (background "#00000000")
        {:-fx-hbar-policy :as-needed
         :-fx-vbar-policy :as-needed}))

(defn text-area-editor-scroll-pane-corner [_]
  (background "#00000000"))

(defn text-area-editor-scroll-pane-scroll-bar [{:keys [scroll-color]}]
  {" .decrement-button" {:-fx-opacity 0}
   " .increment-button" {:-fx-opacity 0}
   ":horizontal" (conj (background "#00000000")
                       {" .track" {:-fx-opacity 0}
                        " .track-background" {:-fx-opacity 0}
                        " .thumb" (background scroll-color)})
   ":vertical" (conj (background "#00000000")
                     {" .track" {:-fx-opacity 0}
                      " .track-background" {:-fx-opacity 0}
                      " .thumb" (background scroll-color)})})

(defn text-area-numbers [{:keys [background-color line-no-color]}]
  (conj (background background-color)
        (highlight background-color)
        (text line-no-color)))

(defn text-area-numbers-text [_]
  {:-fx-text-alignment :right})

(defn text-area-numbers-content [{:keys [background-color]}]
  (background background-color))

(defn text-area-numbers-scroll-pane [{:keys [background-color]}]
  (conj (background background-color)
        {:-fx-hbar-policy :never
         :-fx-vbar-policy :never}))

(defn compose-style-map [color-map] ;this function should also be able to partially reconstruct a map
  (-> (root color-map)
      (assoc-in [".root" "-color-picker"] (color-picker color-map))
      (assoc-in [".root" "-color-picker" " .color-picker-label"] (color-picker-label color-map))
      (assoc-in [".root" "-color-picker" " .combo-box-popup"] (color-picker-combo-popup color-map))
      
      (assoc-in [".root" "-menu-bar"] (menu-bar color-map))
      (assoc-in [".root" "-menu-bar" " .label"] (menu-bar-label color-map))
      (assoc-in [".root" "-menu-bar" "-item"] (menu-bar-item color-map))
      (assoc-in [".root" "-menu-bar" "-item" " .label"] (menu-bar-item-label color-map))
      (assoc-in [".root" "-menu-bar" "-item" "-sub-item"] (menu-bar-sub-item color-map))
      
      (assoc-in [".root" "-fsview"] (fsview color-map))
      (assoc-in [".root" "-fsview" "-button"] (fsview-button color-map))
      (assoc-in [".root" "-fsview" "-filename-input"] (fsview-filename-input color-map))
      (assoc-in [".root" "-fsview" "> .virtual-flow"] (fsview-vflow color-map))
      (assoc-in [".root" "-fsview" "> .virtual-flow" "> .corner"] (fsview-vflow-corner color-map))
      (assoc-in [".root" "-fsview" "> .virtual-flow" "> .scroll-bar"] (fsview-vflow-scroll-bar color-map))
      
      (assoc-in [".root" "-text-area" "-status"] (text-area-status color-map))
      (assoc-in [".root" "-text-area" "-editor"] (text-area-editor color-map))
      (assoc-in [".root" "-text-area" "-editor" " .content"] (text-area-editor-content color-map))
      (assoc-in [".root" "-text-area" "-editor" " .scroll-pane"] (text-area-editor-scroll-pane color-map))
      (assoc-in [".root" "-text-area" "-editor" " .scroll-pane" "> .corner"] (text-area-editor-scroll-pane-corner color-map))
      (assoc-in [".root" "-text-area" "-editor" " .scroll-pane" " .scroll-bar"] (text-area-editor-scroll-pane-scroll-bar color-map))
      (assoc-in [".root" "-text-area" "-numbers"] (text-area-numbers color-map))
      (assoc-in [".root" "-text-area" "-numbers" " *.text"] (text-area-numbers-text color-map))
      (assoc-in [".root" "-text-area" "-numbers" " .content"] (text-area-numbers-content color-map))
      (assoc-in [".root" "-text-area" "-numbers" " .scroll-pane"] (text-area-numbers-scroll-pane color-map))))

(defn make-style-sheet [color-map]
  (css/register ::style (compose-style-map color-map)))

(def style
  (make-style-sheet colors))