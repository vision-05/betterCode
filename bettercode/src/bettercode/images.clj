(ns bettercode.images
  (:require [clojure.java.io :as io]))

(def file-icon (io/input-stream ".bettercode/assets/icons/file.png"))
(def folder-icon (io/input-stream ".bettercode/assets/icons/folder.png"))