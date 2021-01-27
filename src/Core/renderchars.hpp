//! @file

#ifndef RENDER_CHAR_HPP
#define RENDER_CHAR_HPP

#include <SDL2/SDL.h>
#include <immer-0.6.2/immer/flex_vector.hpp>
#include <unordered_map>
#include <array>
#include <string>

#include "datatypes.hpp"

namespace better {

//! better::setPixel sets a specific pixel on the screen for a character, based on the width and height of the characters.
//! This function uses the position of the char in the text buffer being rendered to display it on the window.
//! It also uses a Uint32 color for the foreground and background of each character (RGBA).

void setPixel(SDL_Surface* surface, int x, int y, Uint32 pixel, int column, int row, int charWidth, int charHeight);

//! This function is pretty useless
//! I should probably remove it when refactoring

void createLetter(SDL_Surface* surface, char letter, int column, int row, Uint32 colorfg, Uint32 colorbg, int characterHeight, int characterWidth);

//! better::renderLetter maps the pixels of a letters character map and its position in the text buffer onto the screen.
//! Each letter has a size of 8 pixels wide by 16 pixels long.

void renderLetter(SDL_Surface* surface, std::vector<Uint8>, int column, int row, Uint32 colorfg, Uint32 colorbg, int characterHeight, int characterWidth);

//! better::renderText iterates through the whole text buffer to output the letters to the screen one by one onto the pixel surface
//! This function makes sure that there are only 59 lines rendered and 150 columns rendered on a screen max

void renderText(SDL_Surface* surface, immer::flex_vector<immer::flex_vector<char>> vector, int topLine, int topColumn, const int textHeight, const int textWidth, better::Cursor higlightStart, better::Cursor highlightEnd, Uint32 colorbg, Uint32 colorfg, Uint32 colorhighglight, Uint32 colorparens, Uint32 colorcomments, int columnOffset, int characterHeight, int characterWidth);

//! better::renderCursor renders the text cursor onto the screen, using an offset in the text buffer dictated by the top line being rendered and the first column being rendered on the left of the screen
//! the physical location on the screen of the cursor.
//! The cursor is rendered on the first 2 pixels of the "character" 8x16 grid.

void renderCursor(SDL_Surface* surface, int column, int row, int topLine, int topColumn, int columnOffset, int characterHeight, int characterWidth);

void renderLineNumbers(SDL_Surface* surface, int topLine, int columnOffset, int textLength, int editorHeight, Uint32 colorfg, Uint32 colorbg, int characterHeight, int characterWidth);

//! better::charCheck takes a character and returns its corresponding bitmap.
//! this is passed as a better::charMapArr struct
//! if the character is not an ascii character a filled square will be returned instead.

std::vector<Uint8> charCheck(char letter, std::array<std::vector<Uint8>, 256> letters);

//! better::unpackUint8Bit is used to turn a bit of a Uint8 into a 32 bit value, usable as a pixel.
//! This allows the character maps to be stored in arrays of 16 Uint8s instead of 16x8 Uint32s saving memory space
//! The function simply masks the bit of the number specified by index, if it is set then it is given the foreground color
//! if it is not set it is given the background colour

Uint32 unpackUint8Bit(int index, Uint8 number, Uint32 color, Uint32 colorbg);

//! better::scroll emulates scrolling using a scroll wheel on the text, shifting the topline and topcolumn of the textbuffer to be rendered depending on the direction of the scroll.
//! This topLine and topColumn is used to tell the render function the first line and column to render, so shifting these makes the illusion of scrolling.

}

#endif
