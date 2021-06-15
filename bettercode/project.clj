(defproject bettercode "0.1.0-SNAPSHOT"
  :description "A light text editor made in Clojure with cljfx and C++ with the immer immutable data structures library"
  :url "http://example.com/FIXME"
  :license {:name "EPL-2.0 OR GPL-2.0-or-later WITH Classpath-exception-2.0"
            :url "https://www.eclipse.org/legal/epl-2.0/"}
  :dependencies [[org.clojure/clojure "1.10.1"]
                 [org.clojure/core.cache "1.0.207"]
                 [cljfx "1.7.13"]
                 [cljfx/css "1.1.0"]
                 [aleph "0.4.6"]
                 [gloss "0.2.6"]
                 [manifold "0.1.9-alpha4"]
                 [me.raynes/fs "1.4.6"]
                 [org.openjfx/javafx-base "15.0.1"]
                 [org.fxmisc.richtext/richtextfx "0.10.6"]]
  :main ^:skip-aot bettercode.core
  :target-path "target/%s"
  :profiles {:uberjar {:aot :all
                       :injections [(javafx.application.Platform/exit)]
                       :jvm-opts ["-Dclojure.compiler.direct-linking=true"
                                  "-Xmx50m"]}})
