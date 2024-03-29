(ns bettercode.events
  (:require [cljfx.api :as fx]
            [cljfx.css :as css]
            [clojure.data :as data]
            [clojure.edn :as edn]
            [clojure.string]
            [manifold.stream :as s]
            [bettercode.css]
            [bettercode.meta])
  (:import [javafx.scene.input Clipboard]))

(defn parent-dir [path]
  (clojure.string/join "/" (drop-last (clojure.string/split path #"/"))))

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

(defn get-event-characters [event]
  (-> event
      (.getCharacter)))

(defn get-current-cursor [event]
  (-> event
      (.getSource)
      (.getCaretPosition)))

(defn get-current-anchor [event]
  (-> event
      (.getSource)
      (.getAnchor)))


(defn map-run [atom-map key args]
  ((@atom-map key) args))
(defn validate-handler [handler]
  true) ;make sure handler returns a map

(defn mutate-context [context assoc-fn key value & kvs]
  {:context (fx/swap-context context
                             #(apply assoc-fn % key value kvs))})

(defn take-til-not-bool [tclient]
  (loop [res @(s/take! tclient)]
    (if (not= (class res) java.lang.Boolean) res
        (recur @(s/take! tclient)))))

(def event-handlers (atom {:theme-pick (fn [{:keys [event context]}] (let [filename (.getText (.getTarget event))
                                                                           color-map (edn/read-string (slurp filename))]
                                                                       (spit ".bettercode/meta.edn" (str (assoc bettercode.meta/conf-info :theme-path filename)))
                                                                       (mutate-context context assoc :style-sheet (bettercode.css/make-style-sheet color-map) :colors color-map :theme-picker-show false)))

                           :open-theme-selector (fn [{:keys [context]}] (mutate-context context assoc :theme-picker-show true))

                           :open-theme-creator (fn [{:keys [context]}] (mutate-context context assoc :theme-creator-show true))
                           :close-theme-creator (fn [{:keys [context]}] (mutate-context context assoc :theme-creator-show false))

                           :open-file-explorer (fn [{:keys [context tclient]}] (if (not= (fx/sub-val context :file-path) "") @(s/put! tclient ["save-file" (fx/sub-val context :file-path)])
                                                                                   (println "NOT"))
                                                 (mutate-context context assoc :file-explorer-show true))

                           :style-change (fn [{:keys [context]}] (let [filename (str ".bettercode/" (fx/sub-val context :theme-name-entered) ".edn")]
                                                                   (spit filename (str (fx/sub-val context :colors)))
                                                                   (spit ".bettercode/meta.edn" (str (assoc bettercode.meta/conf-info :theme-path filename)))
                                                                   (mutate-context context assoc :theme-name-entered "" :theme-creator-show false :style-sheet (bettercode.css/make-style-sheet (fx/sub-val context :colors)))))

                           :color-change (fn [{:keys [context key event]}] (mutate-context context assoc-in [:colors key] (str "#" (subs (.toString (.getValue (.getSource event))) 2))))

                           :scroll (fn [{:keys [context event]}] (mutate-context context assoc :vscroll (.getScrollTop (.getSource event))))

                           :type-input (fn [{:keys [context key event]}] (mutate-context context assoc key (.getCharacters (.getSource event))))

                           :new-file-click (fn [{:keys [context tclient]}] (let [file-name (fx/sub-val context :file-name-entered)
                                                                                 file-path-new (str (fx/sub-val context :cur-path) "/" (.toString file-name))
                                                                                 msg @(s/put! tclient ["open-file" file-path-new])
                                                                                 foo @(s/take! tclient)] ;open file will return a value even if we don't use it
                                                                             (mutate-context context assoc :file-path file-path-new :text-editor "" :file-explorer-show false :line-numbers "1\n")))
                           :back-file-click (fn [{:keys [context tclient]}] (let [cur-dir (fx/sub-val context :cur-path)
                                                                                  prev-dir (parent-dir cur-dir)
                                                                                  msg @(s/put! tclient ["get-dir" prev-dir]) ;get parent from :cur-path
                                                                                  result @(s/take! tclient)
                                                                                  dir-contents (if (= result true) @(s/take! tclient) result)]
                                                                              (println dir-contents)
                                                                              (mutate-context context assoc :dir-contents dir-contents :cur-path prev-dir)))
                           :select-file-click (fn [{:keys [context event tclient]}] (let [entry (.getText (.getTarget event))
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
                                                                                                                     (mutate-context context assoc :file-path entry-name :text-editor file-contents :file-explorer-show false :line-numbers line-numbers))))))

                           :text-click (fn [{:keys [context event]}]
                                         (mutate-context context assoc :cursor-pos (get-current-cursor event) :anchor-pos (get-current-anchor event)))

                           :key-press (fn [{:keys [context event tclient]}]
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
                                                                                                                                     @(s/put! tclient ["remove-text" (fx/sub-val context :file-path) cur-cursor-pos (if (= cur-cursor-pos prev-cursor-pos) prev-anchor-pos prev-cursor-pos)]))
                                            (and shortcut? (= "V" code-name)) (let [contents @(fx/on-fx-thread (.getString (Clipboard/getSystemClipboard)))
                                                                                    prev-cursor-pos (fx/sub-val context :cursor-pos)]
                                                                                (println "CONTENTS: " contents)
                                                                                @(s/put! tclient ["insert-text" (fx/sub-val context :file-path) contents prev-cursor-pos])))
                                          (prn (.getText (.getSource event)))
                                          (mutate-context context assoc :cursor-pos (get-current-cursor event) :anchor-pos (get-current-anchor event) :line-numbers line-numbers)))

                           :save-file (fn [{:keys [context tclient]}]
                                        (if (not= (fx/sub-val context :file-path) "") @(s/put! tclient ["save-file" (fx/sub-val context :file-path)])
                                            (println "NOT"))
                                        {:context context})
                           :close-file (fn [{:keys [context tclient]}]
                                         @(s/put! tclient ["close-file" (fx/sub-val context :file-path)])
                                         (mutate-context context assoc :text-editor "" :line-numbers "" :file-path "" :vscroll 0))

                           :save-all-files (fn [{:keys [fx/event fx/context tclient]}]
                                             @(s/put! tclient ["save-all" (fx/sub-val context :file-path) (get-current-text event)])
                                             @(s/take! tclient)
                                             {:context context})})) ;store event handlers in a map. this is as atom so changeable at runtime, allowing custom handlers

(defn event-handler [{:keys [fx/event fx/context event/type key tclient]}]
  (map-run event-handlers type {:event event :context context :key key :tclient tclient}))
