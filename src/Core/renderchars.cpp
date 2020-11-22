#include "renderchars.hpp"

void better::setPixel(SDL_Surface* surface, int x, int y, Uint32 pixel, int column, int row, int charWidth, int charHeight) {
    SDL_LockSurface(surface);
    Uint32 rowOffset = row * surface->w * charHeight; //select the chars pixels by getting the row
    Uint32 location = rowOffset + column * charWidth; //then the column
    Uint32* targetPixel = &(((Uint32*)(surface->pixels))[location + x + y * surface->w]); //then from there select the individual pixel to change
    *targetPixel = pixel;
    SDL_UnlockSurface(surface);
}

void better::createLetter(SDL_Surface* surface, char letter, int column, int row) {
    Uint32 pixelGrid[12][12] {
        {255,255,255,255,255,255,255,255},
        {255,255,255,255,255,255,255,255},
        {255,255,255,255,255,255,255,255},
        {255,255,255,255,255,255,255,255},
        {255,255,255,255,255,255,255,255},
        {255,255,255,255,255,255,255,255},
        {255,255,255,255,255,255,255,255},
        {255,255,255,255,255,255,255,255},
        {255,255,255,255,255,255,255,255},
        {255,255,255,255,255,255,255,255},
        {255,255,255,255,255,255,255,255},
        {255,255,255,255,255,255,255,255}
        };
    for(int i{}; i < 12; ++i) {
        for(int j{}; j < 8; ++j) {
            better::setPixel(surface, i, j, pixelGrid[i][j], column, row, 8, 12); 
        }
    }
}

void better::renderText(SDL_Surface* surface, immer::flex_vector<immer::flex_vector<char>> vector) {
    for(int i{}; i < vector.size(); ++i) {
        for(int j{}; j < vector[i].size(); ++ j) {
            better::createLetter(surface, vector[i][j], j, i);
        }
    }
}