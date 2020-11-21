#define SDL_MAIN_HANDLED

#include <SDL2-2.0.12/include/SDL.h>
#include <immer/flex_vector.hpp>

#include "datatypes.hpp"

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