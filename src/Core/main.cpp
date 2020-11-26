#define SDL_MAIN_HANDLED

#include <SDL2-2.0.12/include/SDL.h>
#include <immer/flex_vector.hpp>
#include <iostream>
#include <vector>

#include "datatypes.hpp"
#include "fileUtils.hpp"
#include "renderchars.hpp"

namespace better {

better::Text verticalNav(better::Text text, SDL_Keycode key);
better::Text horizontalNav(better::Text text, SDL_Keycode key);

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

    
    char filename[] = "main.cpp";
    better::Text firstText {better::readFile(filename), {0,0}, 0, 0};
    texts.push_back(firstText);

    better::renderText(surface, texts.back().textEdit, texts.back().topLineNumber, texts.back().topColumnNumber);
    SDL_UpdateWindowSurface(window);
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0x00, 0x00, 0x00));

    SDL_Event event;
    while(1) {
        while(SDL_PollEvent(&event)) {
            //TODO: create cases for different key/mouse events and link with relevant functions
            if(event.type == SDL_QUIT) {
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return 0;
            }

            else if(event.type == SDL_KEYDOWN && (event.key.keysym.scancode == SDL_SCANCODE_DOWN || event.key.keysym.scancode == SDL_SCANCODE_UP)) {
                texts[texts.size() - 1] = better::verticalNav(texts.back(), event.key.keysym.scancode);
                better::renderText(surface, texts.back().textEdit, texts.back().topLineNumber, texts.back().topColumnNumber);
                SDL_UpdateWindowSurface(window);
                SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0x00, 0x00, 0x00));
            }

            else if(event.type == SDL_KEYDOWN && (event.key.keysym.scancode == SDL_SCANCODE_RIGHT || event.key.keysym.scancode == SDL_SCANCODE_LEFT)) {
                texts[texts.size() - 1] = better::horizontalNav(texts.back(), event.key.keysym.scancode);
                better::renderText(surface, texts.back().textEdit, texts.back().topLineNumber, texts.back().topColumnNumber);
                SDL_UpdateWindowSurface(window);
                SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0x00, 0x00, 0x00));
            }

            else if(event.type == SDL_KEYDOWN) {
                switch(event.key.keysym.sym) {
                    case '\b':
                        texts.push_back(better::backspace(texts.back()));
                        better::renderText(surface, texts.back().textEdit, texts.back().topLineNumber, texts.back().topColumnNumber);
                        SDL_UpdateWindowSurface(window);
                        SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0x00, 0x00, 0x00));
                        break;
                    default:
                        texts.push_back(better::updateText(texts.back(),event.key.keysym.sym)); //save the text at its current state (find out why newline weird behaviour/still printing newline)
                        better::renderText(surface, texts.back().textEdit, texts.back().topLineNumber, texts.back().topColumnNumber);
                        SDL_UpdateWindowSurface(window);
                        SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0x00, 0x00, 0x00));
                }

            }
            better::renderCursor(surface, texts.back().cursor.column, texts.back().cursor.row, 93933, texts.back().topLineNumber, texts.back().topColumnNumber);
        }
    }
    
}

better::Text better::verticalNav(better::Text text, SDL_Keycode key) {
    switch(key) {
        case SDL_SCANCODE_DOWN:
            if(text.cursor.row != text.textEdit.size() - 1) {
                if(text.textEdit[text.cursor.row + 1].size() - 1 < text.cursor.column) { //check the next row has less elements than current column of cursor position
                    text.cursor.column = text.textEdit[text.cursor.row + 1].size() - 1;
                    text.cursor.row += 1;
                }
                else {
                    text.cursor.row += 1;
                }
                if(text.cursor.row == text.topLineNumber + 66) {
                    text.topLineNumber += 1; //scroll down
                }
            }
            break;

        case SDL_SCANCODE_UP:
            if(text.cursor.row) {
                if(text.textEdit[text.cursor.row - 1].size() - 1 < text.cursor.column) { //check the next row has less elements than current column of cursor position
                    text.cursor.column = text.textEdit[text.cursor.row + 1].size() - 1;
                    text.cursor.row -= 1;
                }
                else {
                    text.cursor.row -= 1;
                }
                if(text.cursor.row == text.topLineNumber) {
                    text.topLineNumber -= 1;
                }
            }
            break;
    }

    //TODO: recalculate the cursor pixelIndex here
    return text;
}

better::Text better::horizontalNav(better::Text text, SDL_Keycode key) {
    switch(key) {
        case SDL_SCANCODE_RIGHT:
            if((text.cursor.row != text.textEdit.size() - 1) || (text.cursor.row == text.textEdit.size() - 1 && text.cursor.column != text.textEdit[text.cursor.row].size() - 1)) {
                if(text.cursor.column == text.textEdit[text.cursor.row].size() - 1) {
                    text.cursor.column = 0;
                    text.cursor.row += 1;
                }
                else {
                    text.cursor.column += 1;
                }
                if(text.cursor.column == text.topColumnNumber + 150) {
                    text.topColumnNumber += 1;
                }
            }
            break;

        case SDL_SCANCODE_LEFT:
            if((text.cursor.row) || (!text.cursor.row && text.cursor.column)) {
                if(!text.cursor.column) {
                    text.cursor.row -= 1;
                    text.cursor.column = text.textEdit[text.cursor.row].size() - 1;
                }
                else {
                    text.cursor.column -= 1;
                }
                if(text.cursor.column == text.topColumnNumber) {
                    text.topColumnNumber -= 1;
                }
            }
            break;
    }

    //TODO: increase/decrease cursor pixelIndex here
    return text;
}