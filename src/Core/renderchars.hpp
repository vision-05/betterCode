#ifndef RENDER_CHAR_HPP
#define RENDER_CHAR_HPP

#include <SDL2-2.0.12/include/SDL.h>
#include <immer/flex_vector.hpp>

#include "datatypes.hpp"

namespace better {

void setPixel(SDL_Surface* surface, int x, int y, Uint32 pixel, int column, int row, int charWidth, int charHeight);
void createLetter(SDL_Surface* surface, char letter, int column, int row);
void renderLetter(SDL_Surface* surface, Uint8 pixelGrid[12], int column, int row);
void renderText(SDL_Surface* surface, immer::flex_vector<immer::flex_vector<char>> vector, int topLine, int topColumn);
void renderCursor(SDL_Surface* surface, int column, int row, int topLine, int topColumn);
better::charMapArr charCheck(char letter);
Uint32 unpackUint8Bit(int index, Uint8 number, Uint32 color);

}

#endif