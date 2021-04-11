(defproject bettercodeserver "0.1.0-SNAPSHOT"
  :description "FIXME: write description"
  :url "http://example.com/FIXME"
  :license {:name "EPL-2.0 OR GPL-2.0-or-later WITH Classpath-exception-2.0"
            :url "https://www.eclipse.org/legal/epl-2.0/"}
  :dependencies [[org.clojure/clojure "1.10.1"]
                 [org.clojure/core.cache "1.0.207"]
                 [aleph "0.4.6"]
                 [gloss "0.2.6"]
                 [manifold "0.1.9-alpha4"]
                 [me.raynes/fs "1.4.6"]]
  :main ^:skip-aot bettercodeserver.core
  :target-path "target/%s"
  :profiles {:uberjar {:aot :all
                       :jvm-opts ["-Dclojure.compiler.direct-linking=true"]}})
