# BetterCode

![BetterCode Logo](https://github.com/vision-05/vision-05/logo.svg)

A modern text editor with remote editing built in, made entirely in Clojure using the CLJFX library for UI, and the Gloss, Aleph and Manifold libraries for sockets.

What you will need to compile this project:
JVM, Clojure compiler and leiningen build tool

Status: WIP, development still continuing. 16/02/2021

New architecture:

Client server based model, a text server backend will deal with editing and file IO while a client frontend is in charge of handling events and displaying to the user. The decoupling of the UI from the functionality will allow more flexibility and customisability, as well as the option to write custom clients in any language and remote editing.
