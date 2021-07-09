(ns bettercodeserver.rope
  (:refer-clojure :exclude [concat remove])
  (:require [clojure.string :as string])
  (:import [clojure.lang Associative IPersistentCollection]
           [java.io Writer]))

(set! *warn-on-reflection* true)

(defprotocol IRope
  "Rope DS protocol"
  (split [rope index])
  (concat [rope1 rope2])
  (remove [rope start end]))

(declare leaf-node?)
(declare print-rope)

(declare rope)

(deftype Rope
         [left right weight data meta]
  IRope
  (split [rope index] ;something up with this function? write tests and find out
    (print-rope rope)
    (cond
      (leaf-node? rope) (cond
                          (= 0 index) [(bettercodeserver.rope/rope "") rope]
                          (= (count rope) index) [rope (bettercodeserver.rope/rope "")]
                          :else [(Rope. nil nil index (subs data 0 index) meta)
                                 (Rope. nil nil (- (count data) index) (subs data index) meta)])
      (< index weight) (let [[r1 r2] (split left index)]
                         [r1 (Rope. r2 right (count ^Rope r2) nil meta)])
      :else (let [[r1 r2] (split right (- index weight))]
              [(Rope. left r1 weight nil meta) r2])))

  (concat [rope1 rope2]
    (Rope. rope1 rope2 (count rope1) nil (merge meta (.-meta rope2))))

  (remove [rope start end]
    (let [[begin remaining] (split rope start)
          [_ end] (split remaining (- end start))]
      (concat begin end)))

  Object
  (toString [this]
    (if (leaf-node? this) data
        (str left right)))

  Associative
  (assoc [this index val]
    (if (< index (count this))
        (let [[r1 r2] (split this index)]
          (.concat ^Rope (.cons ^IPersistentCollection r1 val) r2))))

  (entryAt [this key]
    (.valAt ^clojure.lang.ILookup this key))

  (containsKey [this key]
    (cond (< key weight) true
          :else (if (some? data) false
                    (.containsKey ^Associative right (- key weight)))))

  IPersistentCollection
  (count [this]
    (if (leaf-node? this) weight
        (+ weight (.count ^IPersistentCollection right))))

  (cons [this obj]
    (Rope. this (Rope. nil nil (count obj) obj meta) (count this) nil meta))

  (empty [this]
    (Rope. nil nil 0 "" nil))

  (equiv [this rope2]
    (= (str this) (str rope2)))

  clojure.lang.IObj
  (withMeta [this map-arg]
    (Rope. left right weight data map-arg))

  clojure.lang.IMeta
  (meta [this]
    meta)

  clojure.lang.Indexed
  (nth [this key]
    (get this key))
  (nth [this key not-found]
    (if (< key (count this)) (get this key)
        not-found))

  clojure.lang.ILookup
  (valAt [this key]
    (cond (and (<= weight key) (some? right)) (.entryAt ^Associative right (- key weight))
          (some? left) (.entryAt ^Associative left key)
          :else (nth data key)))
  (valAt [this key not-found]
    (if (< key (count this)) (.valAt this key)
        not-found))

  clojure.lang.Seqable
  (seq [this]
    (seq (str this))))

(defn ^:private leaf-node? [rope]
  (some? (.-data ^Rope rope)))

(defmethod print-method Rope [v w]
  (.write ^Writer w (str "#rope \"" v "\"")))

(defmethod print-dup Rope [v w]
  (.write ^Writer w (str "#rope \"" v "\"")))

(defn rope [string]
  (Rope. nil nil (count string) string nil))

(def spaces (map #(if (not= % nil) " " " ") (range 1000000)))
(defn stringify-rope [^Rope rope indent]
  (let [left (.-left rope)
        right (.-right rope)
        space (string/join "" (take indent spaces))]
    (if (leaf-node? rope) (str " <" (.-weight rope) " \"" (string/replace (string/replace (.-data rope) #"\n" "n") #" " "s") "\">")
        (str "\n" space "(" (.-weight rope) " " (when (not= left nil) (stringify-rope left (inc indent))) " " (when (not= right nil) (stringify-rope right (inc indent))) ")\n"))))

(defn print-rope [^Rope rope]
  (println (stringify-rope rope 0)))