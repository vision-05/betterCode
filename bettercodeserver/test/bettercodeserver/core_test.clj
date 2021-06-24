(ns bettercodeserver.core-test
  (:require [clojure.test :as test]
            [bettercodeserver.rope :as rope]))

(def foo {:foo (rope/rope "foo bar baz")})

(test/deftest insertion
  (test/testing "testing rope insertion"
    (test/is (= "foo bar bbbbaz" (str (:foo (-> foo
                                                (assoc-in [:foo 8] "b")
                                                (assoc-in [:foo 9] "b")
                                                (assoc-in [:foo 10] "b")
                                               )))))))

(test/deftest deletion
  (test/testing "testing rope deletion"
    (test/is (= {:foo (rope/rope "fo ba ba")} (assoc-in (assoc-in (assoc-in foo [:foo])))))))