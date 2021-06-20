(ns bettercode.events
  (:require [cljfx.api :as fx]
            [cljfx.css :as css]
            [clojure.data :as data]
            [clojure.edn :as edn]
            [clojure.string]
            [manifold.stream :as s]
            [bettercode.meta]))

(defmulti handle-event :event/type)

(defmethod handle-event :default [e]
  (println "non event"))

(defn parent-dir [path]
  (clojure.string/join "/" (drop-last (clojure.string/split path #"/"))))

(defn fex-back [context tclient]
  (let [cur-dir (fx/sub-val context :cur-path)
        prev-dir (parent-dir cur-dir)
        msg @(s/put! tclient ["get-dir" prev-dir]) ;get parent from :cur-path
        result @(s/take! tclient)
        dir-contents (if (= result true) @(s/take! tclient) result)]
    {:context (fx/swap-context context
                               assoc
                               :dir-contents dir-contents
                               :cur-path prev-dir)}))

(defn get-current-text [event]
  (-> event
      (.getSource)
      (.getParentPopup)
      (.getOwnerWindow)
      (.getScene)
      (.getRoot)
      (.getChildren)
      (.get 1)
      (.getChildren)
      (.get 1)
      (.getChildren)
      (.get 1)
      (.getText)))

(defn make-style-sheet [colors]
  (css/register :bettercode.css/style
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

(defmethod handle-event ::pick-theme [{:keys [fx/event fx/context]}]
  (let [filename (.getText (.getTarget event))
        color-map (edn/read-string (slurp filename))]
    (spit ".bettercode/meta.edn" (str (assoc bettercode.meta/conf-info :theme-path filename)))
    {:context (fx/swap-context context
                               assoc
                               :style-sheet
                               (make-style-sheet color-map)
                               :colors
                               color-map
                               :theme-picker-show
                               false)}))

(defmethod handle-event ::open-selector [{:keys [fx/event fx/context]}]
  {:context (fx/swap-context context
                             assoc
                             :theme-picker-show
                             true)})

(defmethod handle-event ::open-creator [{:keys [fx/event fx/context]}]
  {:context (fx/swap-context context
                             assoc
                             :theme-creator-show
                             true)})

(defmethod handle-event ::close-creator [{:keys [fx/event fx/context]}]
  {:context (fx/swap-context context
                             assoc
                             :theme-creator-show
                             false)})

(defmethod handle-event ::change-style [{:keys [fx/event fx/context]}]
  (let [filename (str ".bettercode/" (fx/sub-val context :theme-name-entered) ".edn")]
    (spit filename (str (fx/sub-val context :colors)))
    (spit ".bettercode/meta.edn" (str (assoc bettercode.meta/conf-info :theme-path filename)))
    {:context (fx/swap-context context
                               assoc
                               :theme-name-entered
                               ""
                               :theme-creator-show
                               false
                               :style-sheet
                               (make-style-sheet (fx/sub-val context :colors)))}))

(defmethod handle-event ::change-color [{:keys [fx/event fx/context key]}]
  {:context (fx/swap-context context
                             assoc-in
                             [:colors key]
                             (str "#" (subs (.toString (.getValue (.getSource event))) 2)))})

(defmethod handle-event ::scroll [{:keys [fx/event fx/context]}]
  {:context (fx/swap-context context
                             assoc
                             :vscroll (.getScrollTop (.getSource event)))})

(defmethod handle-event ::type-name [{:keys [fx/event fx/context key]}]
  {:context (fx/swap-context context
                             assoc
                             key (.getCharacters (.getSource event)))})

(defmethod handle-event ::newclick [{:keys [fx/event fx/context tclient]}]
  (let [file-name (fx/sub-val context :file-name-entered)
        file-path-new (str (fx/sub-val context :cur-path) "/" (.toString file-name))
        msg @(s/put! tclient ["open-file" file-path-new])
        foo @(s/take! tclient)] ;open file will return a value even if we don't use it
    {:context (fx/swap-context context ;how the fuck do I do this
                               assoc
                               :file-path file-path-new
                               :text-editor ""
                               :file-explorer-show false
                               :line-numbers "1\n")}))

(defmethod handle-event ::text-type [{:keys [fx/event fx/context]}]
  (let [no-of-lines (+ 2 (count (re-seq #"\n" (.getText (.getSource event)))))
        line-numbers (apply str (map #(str % \newline) (range 1 no-of-lines)))]
    {:context (fx/swap-context context
                               assoc
                               :line-numbers line-numbers
                               :vscroll (.getScrollTop (.getSource event)))}))

(defmethod handle-event ::openfex [{:keys [fx/event fx/context tclient]}]
  (if (not= (fx/sub-val context :file-path) "") (do
                                                  @(s/put! tclient ["update-buffer" (fx/sub-val context :file-path) (get-current-text event)])
                                                  (println "TAKING " @(s/take! tclient)))
      (println "NOT"))
  {:context (fx/swap-context context
                             assoc
                             :file-explorer-show true)})

(defmethod handle-event ::backclick [{:keys [fx/event fx/context tclient]}]
  (fex-back context tclient))

(defmethod handle-event ::fexclick [{:keys [fx/event fx/context tclient]}]
  (let [entry (.getText (.getTarget event))
        entry-info (subs entry 0 5)
        entry-name (subs entry 5)]
    (cond
      (= entry-info "DIR: ") (do @(s/put! tclient ["get-dir" entry-name])
                                 (let [result @(s/take! tclient)
                                       dir-contents (if (= result true) @(s/take! tclient) result)
                                       foo (println dir-contents)]
                                   {:context (fx/swap-context context
                                                              assoc
                                                              :dir-contents dir-contents
                                                              :cur-path entry-name)}))
      (= entry-info "FIL: ") (do @(s/put! tclient ["open-file" entry-name])
                                 (let [result @(s/take! tclient)
                                       file-contents (if (= result true) @(s/take! tclient) result)
                                       foo (println file-contents)
                                       no-of-lines (+ 2 (count (re-seq #"\n" file-contents)))
                                       line-numbers (apply str (map #(str % \newline) (range 1 no-of-lines)))]
                                   {:context (fx/swap-context context
                                                              assoc
                                                              :file-path entry-name
                                                              :text-editor file-contents
                                                              :file-explorer-show false
                                                              :line-numbers line-numbers)})))))

(defmethod handle-event ::close-file [{:keys [fx/event fx/context tclient]}]
  @(s/put! tclient ["close-file" (fx/sub-val context :file-path)])
  {:context (fx/swap-context context
                             assoc
                             :text-editor ""
                             :line-numbers ""
                             :file-path ""
                             :vscroll 0)})

(defmethod handle-event ::saveevent [{:keys [fx/event fx/context tclient]}]
  @(s/put! tclient ["save-file" (fx/sub-val context :file-path) (get-current-text event)])
  (let [sync-file @(s/take! tclient)]
    {:context (fx/swap-context context
                               assoc
                               :text-editor
                               sync-file)}))

(defmethod handle-event ::save-all [{:keys [fx/event fx/context tclient]}]
  @(s/put! tclient ["save-all" (fx/sub-val context :file-path (get-current-text event))])
  @(s/take! tclient))