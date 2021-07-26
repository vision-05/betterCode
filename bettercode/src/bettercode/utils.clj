(ns bettercode.utils
  (:require [cljfx.api :as fx]
            [manifold.stream :as s]
            [clojure.edn :as edn]
            [clojure.data :as data]
            [cljfx.css :as css]))

(defn get-current-text
  "function that navigates through item heirarchy to get current text-area text from a menu-bar-item-sub-item"
  [event]
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

(defn get-event-characters
  "function that gets characters from a key typed event"
  [event]
  (-> event
      (.getCharacter)))

(defn get-current-cursor
  "function that gets caret position from the text-area"
  [event]
  (-> event
      (.getSource)
      (.getCaretPosition)))

(defn get-current-anchor
  "function that gets anchor from the text area"
  [event]
  (-> event
      (.getSource)
      (.getAnchor)))


(defn parent-dir
  "function that returns the input path without the last part"
  [path]
  (clojure.string/join "/" (drop-last (clojure.string/split path #"/"))))

(defn mutate-context
  "function that mutates a context with the kvs passed to it, just a nice wrapper to lower syntactic bloat"
  [context assoc-fn key value & kvs]
  {:context (fx/swap-context context
                             #(apply assoc-fn % key value kvs))})

(defn take-til-not-bool
  "function that takes data from a socket stream until it is not nil (useful for when uncaught messages are sent to the client)"
  [tclient]
  (loop [res @(s/take! tclient)]
    (if (not= (class res) java.lang.Boolean) res
        (recur @(s/take! tclient)))))
