(ns bettercodeserver.buffer
  (:require [clojure.string :as str]
            [bettercodeserver.rope :as rope]))

(defn add-file
  ([agent-name full-file-path]
   (await agent-name)
   (if (not= nil (@agent-name full-file-path)) (str (@agent-name full-file-path))
       (try (let [contents (slurp full-file-path)]
              (add-file agent-name full-file-path contents))
            (catch java.io.FileNotFoundException e (add-file agent-name full-file-path (rope/rope ""))))))
  ([agent-name full-file-path string]
   (send agent-name assoc full-file-path (rope/rope string))
   string))

(defn remove-file [agent-name full-file-path]
  (send agent-name dissoc full-file-path))

(defn insert-text [agent-name full-file-path text position]
  (if (= (count (@agent-name full-file-path)) position) (send agent-name update full-file-path #(rope/concat % (rope/rope text)))
      (send agent-name assoc-in [full-file-path position] text)))

(defn remove-text [agent-name full-file-path start end]
  (send agent-name update-in [full-file-path] #(rope/remove % start end)))

(defn save-file [agent-name full-file-path]
  (await agent-name)
  (spit full-file-path (@agent-name full-file-path)))

(defn close-all-buffers []
  (shutdown-agents))