#define SDL_MAIN_HANDLED

#include <SDL2-2.0.12/include/SDL.h>
#include <immer/flex_vector.hpp>

#include "datatypes.hpp"
Cursor findCursorPos(SDL_Event& event); //find the cursor position based on x and y of the cursor

int main(int argc, char* argv[]) {
    SDL_SetMainReady();
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Vision Code",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          640,480,0);


    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        if(1) {
            continue;
        }
    }

    SDL_Quit();

    return 0;
}

//create function to update



Cursor findCursorPos(const int topLine, SDL_Event& event) {
    //use line height to determine relative cursor position
    //add to topline to find actual line number
    //use column width to determine column number
    return Cursor {};
}