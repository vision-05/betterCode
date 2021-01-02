//! @file

#ifndef MENUBAR_HPP
#define MENUBAR_HPP

#include <string>
#include <vector>
#include <SDL2-2.0.12/include/SDL.h>

#include "renderchars.hpp"

namespace better {

    void drawMenuBar(SDL_Surface* surface, std::string menus, Uint32 colorfg, Uint32 colorbg, int windowWidth, int columnOffset);
    void drawMenus(SDL_Surface* surface, std::vector<std::string> menus, Uint32 color, Uint32 colorbg, int columnOffset);

}

#endif
