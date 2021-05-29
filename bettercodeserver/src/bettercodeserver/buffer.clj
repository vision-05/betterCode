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

(defn save-file [agent-name full-file-path text]
  (send agent-name assoc full-file-path text)
  (spit full-file-path (@agent-name full-file-path))
  text)

(defn save-all-files [agent-name]
  (let [all-files @agent-name]
    (doseq [file-name all-files]
      (save-file agent-name file-name))))

(defn close-all-buffers []
  (shutdown-agents))