# BetterCode

![BetterCode Logo](https://github.com/vision-05/vision-05/blob/main/logo.svg)

A modern text editor with remote editing built in, made entirely in Clojure using the CLJFX library for UI, and the Gloss, Aleph and Manifold libraries for sockets.

What you will need to compile this project:
JVM, Clojure compiler and leiningen build tool

Status: WIP, development still continuing. 16/02/2021

New architecture:

Client server based model, a text server backend will deal with editing and file IO while a client frontend is in charge of handling events and displaying to the user. The decoupling of the UI from the functionality will allow more flexibility and customisability, as well as the option to write custom clients in any language and remote editing.

<h1> Install & Run </h1>
<h2> Linux </h2>
<h4> First of all you need to download [Leinigen website](https://leiningen.org/ "Leinigen"). You should be able to download it from your distros' repositories. 
 - Ubuntu/Debain based: sudo apt install leinigen
 - Arch: sudo pacman -S lenigen

Then open a terimnal, and navigate the the directory.
To run the server, go to the `bettercodeserver` directory, and use the `lein run` command there
To run the client, go to the `bettercode` folder in the project, and run `lein run [insert ip address]`
Note: You may have to fix the meta.edn file
