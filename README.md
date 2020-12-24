# BetterCode

What you will need to compile this project:
The immer immutable data structures library found here (latest version) https://github.com/arximboldi/immer/releases
SDL2 found here (latest version) https://www.libsdl.org/

makefile example

main: main.cpp datatypes.cpp fileUtils.cpp renderchars.cpp menubar.cpp
	clang++ -O3 -std=c++20 -o BetterCode.exe main.cpp datatypes.cpp fileUtils.cpp renderchars.cpp menubar.cpp -m64 -I"pathToSDLLibInstall/" -I"pathToImmerLibInstall/immer-0.6.2/" -I"pathToImmerLibInstall/immer-0.6.2/immer/" -I"pathToImmerLibInstall/immer-0.6.2/immer/detail/rbts/" -lC:/Users/TFran/VisionCode/src/Libs/SDL2main -lC:/Users/TFran/VisionCode/src/Libs/SDL2