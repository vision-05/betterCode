//! @file
//! This file deals with all the functions for drawing menubars and menus

#include "menubar.hpp"

void better::drawMenuBar(SDL_Surface* surface, std::string menus, Uint32 colorfg, Uint32 colorbg, int windowWidth) {
    SDL_Rect topline {.x = 0, .y = 0, .w = windowWidth, .h = 16};
    int column {};
    SDL_FillRect(surface, &topline, SDL_MapRGBA(surface->format, better::getRed(colorbg), better::getGreen(colorbg), better::getBlue(colorbg), better::getAlpha(colorbg)));
    for(const char& letter : menus) {
        better::renderLetter(surface, better::charCheck(letter).arr, column, 0, colorfg, colorbg);
        ++column;
    }
}

void better::drawMenus(SDL_Surface* surface, std::vector<std::string> menus, Uint32 color, Uint32 colorbg) {
    if(!menus.size()) {
        return;
    }
    const int menuWidth {8 * 20};
    int menuHeight {16 * static_cast<int>(menus.size())};

    SDL_Rect menu {.x = 0, .y = 16, .w = menuWidth, .h = menuHeight};
    SDL_FillRect(surface, &menu, SDL_MapRGBA(surface->format, better::getRed(colorbg), better::getGreen(colorbg), better::getBlue(colorbg), better::getAlpha(colorbg)));

    int column {0};
    int row {1};

    for(int i{}; i < menus.size(); ++i) {
        for(const char& letter : menus[i]) {
            better::renderLetter(surface, better::charCheck(letter).arr, column, row, color, colorbg);
            ++column;
        }
        column = 0;
        ++row;
    }
}
