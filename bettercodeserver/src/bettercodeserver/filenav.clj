(ns bettercodeserver.filenav
  (:require [me.raynes.fs :as fs]))

(defn get-folder-contents
  ([] (get-folder-contents fs/*cwd*))
  ([folder-path] (into (map #(str "DIR: " (.getAbsolutePath %)) (filter fs/directory? (fs/list-dir folder-path)))
                       (map #(str "FIL: " (.getAbsolutePath %)) (filter fs/file? (fs/list-dir folder-path))))))