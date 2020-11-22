#define SDL_MAIN_HANDLED

//may have to remove const keyword in multiple places, test if this is necessary

#include <SDL2-2.0.12/include/SDL.h>
#include <SDL2-2.0.12/include/SDL_ttf.h>
#include <immer/flex_vector.hpp>
#include <iostream>

#include "datatypes.hpp"
#include "fileUtils.hpp"

namespace better {

std::string vectorToString(immer::flex_vector<immer::flex_vector<char>> vector);
SDL_Texture* renderText(better::Text text, SDL_Renderer* renderer, TTF_Font* font);
void renderTextCursor(int column, int row, SDL_Renderer* renderer);

}


int main(int argc, char* argv[]) {
    immer::flex_vector<better::Text> texts {};
    std::cout << "started" << std::endl;
    SDL_SetMainReady();
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window* window = SDL_CreateWindow("Vision Code",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          800,800,0);

    SDL_Renderer* renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    TTF_Font* font = TTF_OpenFont("Libs/Condension Pro.ttf",12);

    SDL_Texture* texture = better::renderText(texts.back(), renderer, font);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, 0, 0);
    SDL_RenderPresent(renderer);

    texts.push_back({better::readFile("fileUtils.hpp"),{0,0},0});

    SDL_Event event;
    while(1) {
        while(SDL_PollEvent(&event)) {
            //TODO: create cases for different key/mouse events and link with relevant functions

            if(event.type == SDL_KEYDOWN) {
                switch(event.key.keysym.sym) {
                    case '\b':
                        texts.push_back(better::backspace(texts.back()));
                        better::renderText(texts.back(), renderer, font);
                        SDL_RenderClear(renderer);
                        SDL_RenderCopy(renderer, texture, 0, 0);
                        SDL_RenderPresent(renderer);
                        break;
                    case '\x1B':
                        SDL_DestroyTexture(texture);
                        TTF_CloseFont(font);
                        TTF_Quit();
                        SDL_DestroyRenderer(renderer);
                        SDL_DestroyWindow(window);
                        SDL_Quit();
                        return 0;
                    default:
                        texts.push_back(better::backspace(texts.back())); //save the text at its current state
                        texture = better::renderText(texts.back(), renderer, font);
                        SDL_RenderClear(renderer);
                        SDL_RenderCopy(renderer, texture, 0, 0);
                        SDL_RenderPresent(renderer);
                }

            }
        }
    }
}

std::string better::vectorToString(immer::flex_vector<immer::flex_vector<char>> vector) {
    std::string str {};
    for(int i{}; i < vector.size(); ++i) {
        for(int j{}; j < vector[i].size(); ++i) {
            str.push_back(vector[i][j]);
        }
    }
    return str;
}

SDL_Texture* better::renderText(better::Text text, SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Color col = {0,0,0,255};
    SDL_Surface* surface = TTF_RenderText_Solid(font, vectorToString(text.textEdit).c_str(), col);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);

    return texture;
}

//TODO: finish these 2 functions to get text rendered

void better::renderTextCursor(int column, int row, SDL_Renderer* renderer) {
    return;
}