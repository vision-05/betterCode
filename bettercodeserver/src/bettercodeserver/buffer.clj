(ns bettercodeserver.buffer
  (:require [clojure.string :as str]))

(defn remove-string [string index length]
  (println "\nSTRING: " string "\nINDEX: " index "\nLENGTH: " length)
  (str (subs string 0 index) (subs string (+ index length))))

(defn insert-string [string insert-str index]
  (str (subs string 0 index) insert-str (subs string index)))

(defn remove-char [string index]
  (str (subs string 0 index) (subs string (inc index))))

(defn add-file
  ([agent-name full-file-path]
   (try (let [contents (slurp full-file-path)]
          (add-file agent-name full-file-path contents))
        (catch java.io.FileNotFoundException e (add-file agent-name full-file-path ""))))
  ([agent-name full-file-path string]
   (send agent-name assoc full-file-path string)
   string))

(defn remove-file [agent-name full-file-path]
  (send agent-name dissoc full-file-path))

(defn add-string [agent-name full-file-path position insert-str]
  (send agent-name update-in [full-file-path] insert-string insert-str position))

(defn del-string [agent-name full-file-path position length]
  (send agent-name update-in [full-file-path] remove-string position length))

(defn del-char [agent-name full-file-path position]
  (send agent-name update-in [full-file-path] remove-char (- position 1)))

(defn text-edit [agent-name full-file-path string index length]
  (println "length:" length)
  (cond
    (and (= string "\b") (> index 0) (= nil length)) (del-char agent-name full-file-path (+ index 1))
    (and (= string "\b") (> index 0)) (del-string agent-name full-file-path index length)
    (and (> index -1) (not= string "\b")) (add-string agent-name full-file-path (- index (count string)) string)
    :else false))

(defn save-file [agent-name full-file-path]
  (spit full-file-path (@agent-name full-file-path)))

(defn save-all-files [agent-name]
  (let [all-files @agent-name]
    (doseq [file-name all-files]
      (save-file agent-name file-name))))

(defn close-all-buffers []
  (shutdown-agents))