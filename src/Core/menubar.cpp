//! @file
//! This file deals with all the functions for drawing menubars and menus

#include "menubar.hpp"

void better::drawMenuBar(SDL_Surface* surface, std::string menus, Uint32 colorfg, Uint32 colorbg, int windowWidth) {
    SDL_Rect topline;
    topline.h = 16;
    topline.w = windowWidth;
    topline.x = 0;
    topline.y = 0;
    int column {};
    SDL_FillRect(surface, &topline, colorbg);
    for(const char& letter : menus) {
        better::renderLetter(surface, better::charCheck(letter).arr, column, 0, colorfg, colorbg);
        ++column;
    }
}