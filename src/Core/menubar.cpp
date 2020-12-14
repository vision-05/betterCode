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

void better::drawMenus(SDL_Surface* surface, std::vector<std::string> menus, Uint32 color, Uint32 colorbg) {
    if(!menus.size()) {
        return;
    }
    const int menuWidth {8 * 20};
    int menuHeight {16 * static_cast<int>(menus.size())};

    SDL_Rect menu;
    menu.h = menuHeight;
    menu.w = menuWidth;
    menu.x = 0;
    menu.y = 16;
    SDL_FillRect(surface, &menu, colorbg);

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