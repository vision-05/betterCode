(ns bettercodeserver.core-test
  (:require [clojure.test :as test]
            [bettercodeserver.core :as core]
            [bettercodeserver.buffer :as buffer]))

(def files (agent {}))
(def filename "/home/tim/foo.txt")
(def filename-two "/home/tim/bar.txt")

(defn send-await [f agent & args]
  (apply f agent args)
  (await agent))

(test/deftest agent-buffer-add-file
  (test/testing "Adding file to agent"
    (send-await buffer/add-file files filename)
    (test/is (= (@files filename) (slurp filename)))
    (send-await buffer/add-file files filename-two)
    (test/is (= (@files filename-two) ""))))

(test/deftest agent-buffer-add-string
  (test/testing "Adding text to files"
    (send-await buffer/add-string files filename 1 "ello World")
    (test/is (= (@files filename) "Hello World\n"))))

(test/deftest agent-buffer-remove-string
  (test/testing "Removing text from files"
    (send-await buffer/del-string files filename 0 11)
    (test/is (= (@files filename) "\n"))))

(test/deftest agent-buffer-remove-char
  (test/testing "Removing character from file"))

(test/deftest agent-buffer-save-file
  (test/testing "Saving file from agent buffer"))

(test/deftest agent-buffer-remove-file
  (test/testing "Removing a file from an agent"
    (send-await buffer/remove-file files filename)
    (test/is (= (@files filename) nil))))
