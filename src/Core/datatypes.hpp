#ifndef DATATYPES_HPP
#define DATATYPES_HPP

#include <SDL2-2.0.12/include/SDL.h>
#include <immer/flex_vector.hpp>

namespace better {

struct Cursor { //store the line number and column number of the cursor
    int row;
    int column;
};

better::Cursor findCursorPos(int topLine, SDL_Event& event);

struct Text{
    immer::flex_vector<immer::flex_vector<char>> textEdit; //keeps the text state
    better::Cursor cursor; //keeps the cursor state
    int topLineNumber; //keeps top line number so can calculate new line being clicked on
    int topColumnNumber;
};

better::Text updateText(better::Text textEdit, char newChar);
better::Text backspace(better::Text textEdit);

}

#endif