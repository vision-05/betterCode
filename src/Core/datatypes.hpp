#ifndef DATATYPES_HPP
#define DATATYPES_HPP

#include <immer/flex_vector.hpp>

struct Cursor { //store the line number and column number of the cursor
    int yPos;
    int xPos;
};

struct Text{
    immer::flex_vector<immer::flex_vector<char>> textEdit; //keeps the text state
    Cursor cursor; //keeps the cursor state
    int topLineNumber; //keeps top line number so can calculate new line being clicked on
};

const Text updateText(const Text textEdit);

#endif