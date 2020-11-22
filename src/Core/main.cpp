#define SDL_MAIN_HANDLED

#include <SDL2-2.0.12/include/SDL.h>
#include <immer/flex_vector.hpp>
#include <iostream>
#include <vector>

#include "datatypes.hpp"
#include "fileUtils.hpp"
#include "renderchars.hpp"

namespace better {

void renderTextCursor(int column, int row, SDL_Renderer* renderer);

}

int main(int argc, char* argv[]) {
    SDL_SetMainReady();
    SDL_Init(SDL_INIT_VIDEO);
    std::vector<better::Text> texts {};

    SDL_Window* window = SDL_CreateWindow("Better Code",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          1200,800,0);
    

    SDL_Renderer* renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Surface* surface = SDL_GetWindowSurface(window);
    SDL_PixelFormat* format;
    format = surface->format;

    
    char filename[] = "fileUtils.hpp";
    better::Text firstText {better::readFile(filename), {0,0}, 0};
    texts.push_back(firstText);

    better::renderText(surface, texts.back().textEdit);
    SDL_UpdateWindowSurface(window);
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0x00, 0x00, 0x00));

    SDL_Event event;
    while(1) {
        while(SDL_PollEvent(&event)) {
            //TODO: create cases for different key/mouse events and link with relevant functions

            if(event.type == SDL_KEYDOWN) {
                switch(event.key.keysym.sym) {
                    case '\b':
                        texts.push_back(better::backspace(texts.back()));
                        better::renderText(surface, texts.back().textEdit);
                        SDL_UpdateWindowSurface(window);
                        SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0x00, 0x00, 0x00));
                        break;
                    case '\x1B':
                        SDL_DestroyRenderer(renderer);
                        SDL_DestroyWindow(window);
                        SDL_Quit();
                        return 0;
                    default:
                        texts.push_back(better::updateText(texts.back(),event.key.keysym.sym)); //save the text at its current state
                        better::renderText(surface, texts.back().textEdit);
                        SDL_UpdateWindowSurface(window);
                        SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0x00, 0x00, 0x00));
                }

            }
        }
    }
    
}

void better::renderTextCursor(int column, int row, SDL_Renderer* renderer) {
    return;
}