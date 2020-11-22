#ifndef RENDER_CHAR_HPP
#define RENDER_CHAR_HPP

#include <SDL2-2.0.12/include/SDL.h>
#include <immer/flex_vector.hpp>

namespace better {

void setPixel(SDL_Surface* surface, int x, int y, Uint32 pixel, int column, int row, int charWidth, int charHeight);
void createLetter(SDL_Surface* surface, char letter, int column, int row);
void renderText(SDL_Surface* surface, immer::flex_vector<immer::flex_vector<char>> vector);

}

#endif