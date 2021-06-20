(ns bettercode.meta
  (:require [clojure.edn :as edn]
            [me.raynes.fs :as fs]))

(def conf-info (edn/read-string (slurp ".bettercode/meta.edn")))

(defn get-themes []
  (into [] (map #(str (.getAbsolutePath %)) (filter fs/file? (fs/list-dir ".bettercode/")))))