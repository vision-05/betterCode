# BetterCode

What you will need to compile this project:
The immer immutable data structures library found here (latest version) https://github.com/arximboldi/immer/releases

Status: WIP, development still continuing. 16/02/2021

New architecture:

Client server based model, a text server backend will deal with editing and file IO while a client frontend is in charge of handling events and displaying to the user. The decoupling of the UI from the functionality will allow more flexibility and customisability, as well as the option to write custom clients in any language and remote editing.
