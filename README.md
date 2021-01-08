# BetterCode

What you will need to compile this project:
The immer immutable data structures library found here (latest version) https://github.com/arximboldi/immer/releases
SDL2 found here (latest version) https://www.libsdl.org/

makefile example

main: main.cpp datatypes.cpp fileUtils.cpp renderchars.cpp menubar.cpp
	clang++ -O3 -std=c++20 -o BetterCode.exe main.cpp datatypes.cpp fileUtils.cpp renderchars.cpp menubar.cpp -m64 -I"pathToImmerLibInstall/immer-0.6.2/" -lSDL2main -lSDL2
