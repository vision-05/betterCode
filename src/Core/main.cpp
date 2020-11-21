#define SDL_MAIN_HANDLED

#include <SDL2-2.0.12/include/SDL.h>
#include <SDL2-2.0.12/include/SDL_ttf.h>
#include <immer/flex_vector.hpp>

#include "datatypes.hpp"

namespace better {

int renderLine(const immer::flex_vector<char> line, int row, bool cursor, SDL_Renderer* renderer);
int renderText(const better::Text text, SDL_Renderer* renderer);
int renderTextCursor(int column, int row, SDL_Renderer* renderer);
int renderChar(char letter, int column, int row, SDL_Renderer* renderer);

}


int main(int argc, char* argv[]) {
    SDL_SetMainReady();
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Vision Code",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          800,800,0);

    SDL_Renderer* renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);


    SDL_Event event;
    while(1) {
        while(SDL_PollEvent(&event)) {
            //TODO: create cases for different key/mouse events and link with relevant functions

            if(event.type == SDL_KEYDOWN) {
                if(event.key.keysym.sym == 'q') {
                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_Quit();
                    return 0;
                }
            }
        }
    }
}

int better::renderLine(const immer::flex_vector<char> line, int row, bool cursor, SDL_Renderer* renderer) {
    for(int column{}; column < line.size(); ++column) {
        if(cursor) {
            better::renderTextCursor(column, row, renderer); //render the cursor at the specified column
        }
        better::renderChar(line[column], column, row, renderer);
    }
}

int better::renderText(const better::Text text, SDL_Renderer* renderer) {
    for(int row{}; row < text.textEdit.size(); ++row) {
        bool cursorRow = false;
        if(row == text.cursor.row) { //if cursor on this line set cursor to true
            cursorRow = true;
        }
        better::renderLine(text.textEdit[row], row, cursorRow, renderer);
    }
}

//TODO: finish these 2 functions to get text rendered

int better::renderTextCursor(int column, int row, SDL_Renderer* renderer) {

}

int better::renderChar(char letter, int column, int row, SDL_Renderer* renderer) {

}