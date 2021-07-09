(ns bettercode.core-test
  (:require [clojure.test :refer :all]
            [bettercode.core :refer :all]
            [bettercode.css :refer :all]))

(deftest a-test
  (testing "FIXME, I fail."
    (is (= (compose-style-map colors) style))))
