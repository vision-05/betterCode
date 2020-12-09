//! @file

#ifndef MENUBAR_HPP
#define MENUBAR_HPP

#include <string>
#include <SDL2-2.0.12/include/SDL.h>

#include "renderchars.hpp"

namespace better {

    void drawMenuBar(SDL_Surface* surface, std::string menus, Uint32 colorfg, Uint32 colorbg, int windowWidth);
    void drawMenus(SDL_Surface* surface, std::string menus[], Uint32 color, Uint32 colorbg);

}

#endif