# BetterCode

![BetterCode Logo](https://github.com/vision-05/vision-05/blob/main/logo.svg)

A modern text editor with remote editing built in, made entirely in Clojure using the CLJFX library for UI, and the Gloss, Aleph and Manifold libraries for sockets.

What you will need to compile this project:
JVM, Clojure compiler and leiningen build tool

Status: WIP, development still continuing. 16/02/2021

New architecture:

Client server based model, a text server backend will deal with editing and file IO while a client frontend is in charge of handling events and displaying to the user. The decoupling of the UI from the functionality will allow more flexibility and customisability, as well as the option to write custom clients in any language and remote editing.

<h1> Install </h1>
<h3> Linux </h3>
<p> First of all you need to download
<a herf="https://leiningen.org/"> Leiningen </a>. You should be able to download it from your distros' repositories.
<ul>
 <li>Ubuntu/Debain based: `sudo apt install leiningen`</li>
 <li>Arch: `sudo pacman -S leiningen` </li>
</ul>

Then clone the repository using `git clone https://github.com/vision-05/betterCode.git`
That is everything that's needed!
<h1> Run </h1>
<h3> Running Server </h3>
<ol>
<li> Open a terminal/command line session</l1>
<li> Navigate to the directory where you cloned the repository to</li>
<li> Go into the `bettercodeserver` directory </li>
<li> Run the command `lein run` </li>
</ol>

<h3> Running client </h3>
<ol>
<li> Open a terminal/command line session</l1>
<li> Navigate to the directory where you cloned the repository to</li>
<li> Navigate into the `bettercode` directory </li>
<li> Run the command `lein run [server ip]` insert the server ip where it says( Example `lein run 192.168.4.16`) </li>

