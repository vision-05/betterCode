(ns bettercode.handlers
  (:require [cljfx.api :as fx]
            [manifold.stream :as s]
            [clojure.edn :as edn]
            [clojure.data :as data]
            [cljfx.css :as css]
            [bettercode.utils :refer :all]
            [bettercode.css]
            [bettercode.meta])
  (:import [javafx.scene.input Clipboard]))

(defn theme-pick [{:keys [event context]}]
  (let [filename  (.getText (.getTarget event))
        color-map (edn/read-string (slurp filename))]
    (spit ".bettercode/meta.edn" (str (assoc bettercode.meta/conf-info :theme-path filename)))
    (mutate-context context assoc :style-sheet (bettercode.css/make-style-sheet color-map) :colors color-map :theme-picker-show false)))

(defn open-theme-selector [{:keys [context]}] (mutate-context context assoc :theme-picker-show true))

(defn open-theme-creator [{:keys [context]}] (mutate-context context assoc :theme-creator-show true))

(defn close-theme-creator [{:keys [context]}] (mutate-context context assoc :theme-creator-show false))

(defn open-file-explorer [{:keys [context tclient]}]
  (if (not= (fx/sub-val context :file-path) "") @(s/put! tclient ["save-file" (fx/sub-val context :file-path)])
      (println "NOT"))
  (mutate-context context assoc :file-explorer-show true))

(defn style-change [{:keys [context]}]
  (let [filename (str ".bettercode/" (fx/sub-val context :theme-name-entered) ".edn")]
    (spit filename (str (fx/sub-val context :colors)))
    (spit ".bettercode/meta.edn" (str (assoc bettercode.meta/conf-info :theme-path filename)))
    (mutate-context context assoc
                    :theme-name-entered ""
                    :theme-creator-show false
                    :style-sheet (bettercode.css/make-style-sheet (fx/sub-val context :colors)))))

(defn color-change [{:keys [context key event]}]
  (mutate-context context assoc-in [:colors key] [(str "#" (subs (.toString (.getValue (.getSource event))) 2)) "normal"]))

(defn scroll [{:keys [context event]}]
  (mutate-context context assoc :vscroll (.getScrollTop (.getSource event))))

(defn type-input [{:keys [context key event]}]
  (mutate-context context assoc key (.getCharacters (.getSource event))))

(defn new-file-click [{:keys [context tclient]}]
  (let [file-name (fx/sub-val context :file-name-entered)
        file-path-new (str (fx/sub-val context :cur-path) "/" (.toString file-name))
        msg @(s/put! tclient ["open-file" file-path-new])
        foo @(s/take! tclient)] ;open file will return a value even if we don't use it
    (mutate-context context assoc :file-path file-path-new :text-editor "" :file-explorer-show false :line-numbers "1\n")))

(defn back-file-click [{:keys [context tclient]}] (let [cur-dir (fx/sub-val context :cur-path)
                                                        prev-dir (parent-dir cur-dir)
                                                        msg @(s/put! tclient ["get-dir" prev-dir]) ;get parent from :cur-path
                                                        result @(s/take! tclient)
                                                        dir-contents (if (= result true) @(s/take! tclient) result)]
                                                    (println dir-contents)
                                                    (mutate-context context assoc :dir-contents dir-contents :cur-path prev-dir)))

(defn select-file-click [{:keys [context event tclient]}] (let [entry (.getText (.getTarget event))
                                                                entry-info (subs entry 0 5)
                                                                entry-name (subs entry 5)]
                                                            (cond
                                                              (= entry-info "DIR: ") (do @(s/put! tclient ["get-dir" entry-name])
                                                                                         (let [dir-contents (take-til-not-bool tclient)]
                                                                                           (mutate-context context assoc :dir-contents dir-contents :cur-path entry-name)))
                                                              (= entry-info "FIL: ") (do @(s/put! tclient ["open-file" entry-name])
                                                                                         (let [file-contents (take-til-not-bool tclient)
                                                                                               bar (println file-contents)
                                                                                               no-of-lines (+ 2 (count (re-seq #"\n" file-contents)))
                                                                                               line-numbers (apply str (map #(str % \newline) (range 1 no-of-lines)))]
                                                                                           (println (fx/sub-val context :spans))
                                                                                           (mutate-context context
                                                                                                           assoc
                                                                                                           :file-path entry-name
                                                                                                           :text-editor file-contents
                                                                                                           :file-explorer-show false
                                                                                                           :line-numbers line-numbers
                                                                                                           :spans [{:start 0
                                                                                                                   :end (count file-contents)
                                                                                                                   :style "text-color"}]))))))


(defn text-click [{:keys [context event]}]
  (mutate-context context assoc :cursor-pos (get-current-cursor event) :anchor-pos (get-current-anchor event)))

(defn key-press [{:keys [context event tclient]}]
  (let [text (.getText event)
        shortcut? (.isShortcutDown event)
        code (.getCode event)
        code-name (.getName code)
        no-of-lines (+ 2 (count (re-seq #"\n" (.getText (.getSource event)))))
        line-numbers (apply str (map #(str % \newline) (range 1 no-of-lines)))]
    (println text shortcut? code code-name)
    (cond
      (and (not shortcut?) (not= code-name "Backspace")) (let [prev-cursor-pos (fx/sub-val context :cursor-pos)]
                                                           @(s/put! tclient ["insert-text" (fx/sub-val context :file-path) (if (= text "\r") "\n" text) prev-cursor-pos]))
      (or (and shortcut? (= "X" code-name)) (and (not shortcut?) (= "Backspace" code-name))) (let [prev-cursor-pos (fx/sub-val context :cursor-pos)
                                                                                                   prev-anchor-pos (fx/sub-val context :anchor-pos)
                                                                                                   cur-cursor-pos (get-current-cursor event)]
                                                                                               @(s/put! tclient ["remove-text"
                                                                                                                 (fx/sub-val context :file-path)
                                                                                                                 cur-cursor-pos
                                                                                                                 (if (= cur-cursor-pos prev-cursor-pos) prev-anchor-pos prev-cursor-pos)]))
      (and shortcut? (= "V" code-name)) (let [contents @(fx/on-fx-thread (.getString (Clipboard/getSystemClipboard)))
                                              prev-cursor-pos (fx/sub-val context :cursor-pos)]
                                          (println "CONTENTS: " contents)
                                          @(s/put! tclient ["insert-text" (fx/sub-val context :file-path) contents prev-cursor-pos])))
    (prn (.getLength (.getSource event)))
    (prn (.getSource event))
    ;;FIXME invalid ranges for some reason. Figure out cause. Refactor this function
    (mutate-context context assoc :spans [(assoc ((fx/sub-val context :spans) 0) :end (.getLength (.getSource event)))] :cursor-pos (get-current-cursor event) :anchor-pos (get-current-anchor event) :line-numbers line-numbers)))

(defn save-file [{:keys [context tclient]}]
  (if (not= (fx/sub-val context :file-path) "") @(s/put! tclient ["save-file" (fx/sub-val context :file-path)])
      (println "NOT"))
  {:context context})

(defn close-file [{:keys [context tclient]}]
  @(s/put! tclient ["close-file" (fx/sub-val context :file-path)])
  (mutate-context context assoc :text-editor "" :line-numbers "" :file-path "" :vscroll 0))

(defn save-all-files [{:keys [fx/event fx/context tclient]}]
  @(s/put! tclient ["save-all" (fx/sub-val context :file-path) (get-current-text event)])
  @(s/take! tclient)
  {:context context})
