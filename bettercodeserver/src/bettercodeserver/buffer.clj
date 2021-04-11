(ns bettercodeserver.buffer
  (:require [clojure.string :as str]))

(defn remove-string [string index length]
  (str (subs string 0 index) (subs string (+ index length))))

(defn insert-string [string insert-str index]
  (str (subs string 0 index) insert-str (subs string index)))

(defn remove-char [string index]
  (str (subs string 0 index) (subs string (inc index))))

(defn add-file
  ([agent-name full-file-path]
   (send agent-name assoc full-file-path (slurp full-file-path)))
  ([agent-name full-file-path string]
   (send agent-name assoc full-file-path string)))

(defn remove-file [agent-name full-file-path]
  (send agent-name dissoc full-file-path))

(defn add-string [agent-name full-file-path position insert-str]
  (send agent-name update-in [full-file-path] insert-string insert-str position))

(defn del-string [agent-name full-file-path position length]
  (send agent-name update-in [full-file-path] remove-string position length))

(defn del-char [agent-name full-file-path position]
  (send agent-name update-in [full-file-path] remove-char position))

(defn close-all-buffers []
  (shutdown-agents))