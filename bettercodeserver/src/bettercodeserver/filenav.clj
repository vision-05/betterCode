(ns bettercodeserver.filenav
  (:require [me.raynes.fs :as fs]))

(defn get-folder-contents
  ([] (get-folder-contents fs/*cwd*))
  ([folder-path] {:dirs (filter fs/directory? (fs/list-dir folder-path))
                  :files (filter fs/file? (fs/list-dir folder-path))}))