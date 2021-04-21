(ns bettercodeserver.core-test
  (:require [clojure.test :as test]
            [bettercodeserver.core :as core]
            [bettercodeserver.buffer :as buffer]))

(def files (agent {}))
(def filename "/home/tim/foo.txt")

(test/deftest agent-buffer-add-file
  (test/testing "Adding file to agent"
    (buffer/add-file files filename)
    (await files)
    (test/is (= (@files filename) (slurp filename)))))

(test/deftest agent-buffer-add-string
  (test/testing "Adding text to files"
    (buffer/add-string files filename 1 "ello World")
    (await files)
    (test/is (= (@files filename) "Hello World\n"))))

(test/deftest agent-buffer-remove-string
  (test/testing "Removing text from files"
    (buffer/del-string files filename 0 11)
    (await files)
    (test/is (= (@files filename) "\n"))))

(test/deftest agent-buffer-remove-char
  (test/testing "Removing character from file"))

(test/deftest agent-buffer-save-file
  (test/texting "Saving file from agent buffer"))

(test/deftest agent-buffer-remove-file
  (test/testing "Removing a file from an agent"
    (buffer/remove-file files filename)
    (await files)
    (test/is (= (@files filename) nil))))
