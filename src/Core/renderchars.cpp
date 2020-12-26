//! @file
//! This file handles all of the text rendering functions

#include "renderchars.hpp"
//TODO: symbols have different colours to text

namespace better {
    static std::array<better::charMapArr, 95> letters {
        {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //Space
        {0,8,8,8,8,8,8,0,0,8,8,0,0,0,0,0}, //!
        {0,20,20,20,0,0,0,0,0,0,0,0,0,0,0,0}, //"
        {0,54,54,127,127,54,54,127,127,54,54,0,0,0,0,0}, //# make thinner
        {16,124,20,18,20,20,24,112,80,80,62,16,0,0,0,0}, //$ make go down
        {0,78,42,46,16,16,8,8,116,84,114,0,0,0,0,0}, //%
        {0,28,34,34,18,20,8,84,52,50,42,68,0,0,0,0}, //&
        {0,8,8,8,0,0,0,0,0,0,0,0,0,0,0,0}, //'
        {32,32,16,16,8,8,8,8,8,8,8,8,16,16,32,32}, //(
        {4,4,8,8,16,16,16,16,16,16,16,16,8,8,4,4}, //)
        {0,42,20,62,20,42,0,0,0,0,0,0,0,0,0,0}, //*
        {0,0,0,0,0,8,8,62,8,8,0,0,0,0,0,0}, //+
        {0,0,0,0,0,0,0,0,0,8,8,4,0,0,0,0}, //,
        {0,0,0,0,0,0,0,62,0,0,0,0,0,0,0,0}, //-
        {0,0,0,0,0,0,0,0,0,8,8,0,0,0,0,0}, //.
        {0,64,32,32,16,16,8,8,4,4,2,0,0,0,0,0}, ///
        {0,24,36,36,66,114,78,66,36,36,24,0,0,0,0,0}, //0
        {0,16,24,20,16,16,16,16,16,16,60,0,0,0,0,0}, //1
        {0,24,36,68,66,64,32,32,16,12,126,0,0,0,0,0}, //2
        {0,14,48,32,32,28,16,32,32,48,14,0,0,0,0,0}, //3
        {0,32,48,48,40,40,36,126,32,32,32,0,0,0,0,0}, //4
        {0,62,2,2,2,14,16,32,32,16,14,0,0,0,0,0}, //5
        {0,48,8,4,4,26,34,66,34,36,24,0,0,0,0,0}, //6
        {0,62,32,32,16,16,8,8,4,4,2,0,0,0,0,0}, //7
        {0,24,36,66,36,24,36,66,66,36,24,0,0,0,0,0}, //8
        {0,24,36,66,66,68,120,64,32,32,28,0,0,0,0,0}, //9
        {0,0,0,8,8,0,0,0,8,8,0,0,0,0,0,0}, //:
        {0,0,0,8,8,0,0,0,0,8,8,4,0,0,0,0}, //;
        {0,0,96,24,6,1,1,6,24,96,0,0,0,0,0,0}, //<
        {0,0,0,0,0,0,126,0,126,0,0,0,0,0,0,0}, //=
        {0,0,1,6,24,96,96,24,6,1,0,0,0,0,0,0}, //>
        {0,12,16,32,32,16,8,8,0,8,8,0,0,0,0,0}, //?
        {0,0,24,36,90,165,165,165,57,66,36,24,0,0,0,0}, //@
        {0,0,24,24,36,36,90,66,66,66,66,0,0,0,0,0}, //A
        {0,0,14,18,18,14,18,34,34,18,14,0,0,0,0,0}, //B
        {0,0,112,12,6,2,2,2,6,12,112,0,0,0,0,0}, //C
        {0,0,14,18,34,34,34,34,34,18,14,0,0,0,0,0}, //D
        {0,0,62,2,2,2,30,2,2,2,62,0,0,0,0,0}, //E
        {0,0,62,2,2,2,62,2,2,2,2,0,0,0,0,0}, //F
        {0,0,56,4,2,2,58,34,34,36,56,0,0,0,0,0}, //G
        {0,0,34,34,34,34,62,34,34,34,34,0,0,0,0,0}, //H
        {0,0,28,8,8,8,8,8,8,8,28,0,0,0,0,0}, //I
        {0,0,62,32,32,32,32,32,34,20,8,0,0,0,0,0}, //J
        {0,0,34,18,10,6,6,10,18,18,34,0,0,0,0,0}, //K
        {0,0,2,2,2,2,2,2,2,2,62,0,0,0,0,0}, //L
        {0,0,66,66,102,102,90,66,66,66,66,0,0,0,0,0}, //M
        {0,0,34,34,38,38,42,42,50,50,34,0,0,0,0,0}, //N
        {0,0,24,36,66,66,66,66,66,36,24,0,0,0,0,0}, //O
        {0,0,30,34,34,34,34,30,2,2,2,0,0,0,0,0}, //P
        {0,0,24,36,66,66,66,66,66,36,24,112,0,0,0,0}, //Q
        {0,0,14,18,18,18,14,2,6,10,18,0,0,0,0,0}, //R
        {0,0,120,4,2,4,24,32,64,64,62,0,0,0,0,0}, //S
        {0,0,62,8,8,8,8,8,8,8,8,0,0,0,0,0}, //T
        {0,0,66,66,66,66,66,66,66,36,24,0,0,0,0,0}, //U
        {0,0,66,66,66,66,36,36,36,24,24,0,0,0,0,0}, //V
        {0,0,66,66,66,66,90,102,102,66,66,0,0,0,0,0}, //W
        {0,0,66,66,36,36,24,36,36,66,66,0,0,0,0,0}, //X
        {0,0,34,34,20,20,8,8,8,8,8,0,0,0,0,0}, //Y
        {0,0,126,64,32,32,16,8,8,4,126,0,0,0,0,0}, //Z
        {28,4,4,4,4,4,4,4,4,4,4,4,4,4,4,28}, //[
        {0,2,4,4,8,8,16,16,32,32,64,0,0,0,0,0}, //Backslash
        {56,32,32,32,32,32,32,32,32,32,32,32,32,32,32,56}, //]
        {0,8,20,34,0,0,0,0,0,0,0,0,0,0,0,0}, //^
        {0,0,0,0,0,0,0,0,0,0,127,0,0,0,0,0}, //_
        {0,6,8,0,0,0,0,0,0,0,0,0,0,0,0,0}, //`
        {0,0,0,0,0,28,34,60,34,50,44,0,0,0,0,0}, //a
        {0,2,2,2,2,30,34,34,34,34,30,0,0,0,0,0}, //b
        {0,0,0,0,0,56,4,4,4,4,56,0,0,0,0,0}, //c
        {0,32,32,32,32,60,34,34,34,50,44,0,0,0,0,0}, //d
        {0,0,0,0,0,28,34,62,2,2,60,0,0,0,0,0}, //e
        {0,48,8,4,30,4,4,4,4,4,4,0,0,0,0,0}, //f
        {0,0,0,0,0,60,18,18,18,12,2,28,34,34,28,0}, //g
        {0,2,2,2,2,26,38,34,34,34,34,0,0,0,0,0}, //h
        {0,16,16,0,0,24,16,16,16,16,56,0,0,0,0,0}, //i
        {0,16,16,0,0,30,16,16,16,16,16,16,16,16,12,0}, //j
        {0,2,2,2,2,50,10,6,10,18,34,0,0,0,0,0}, //k
        {0,14,8,8,8,8,8,8,8,8,62,0,0,0,0,0}, //l
        {0,0,0,0,0,36,90,90,90,90,90,0,0,0,0,0}, //m
        {0,0,0,0,0,26,38,34,34,34,34,0,0,0,0,0}, //n
        {0,0,0,0,0,28,34,34,34,34,28,0,0,0,0,0}, //o
        {0,0,0,0,0,30,34,34,34,34,30,2,2,2,2,0}, //p
        {0,0,0,0,0,60,34,34,34,34,60,32,32,32,32,0}, //q
        {0,0,0,0,0,26,38,2,2,2,2,0,0,0,0,0}, //r
        {0,0,0,0,0,60,2,28,32,32,30,0,0,0,0,0}, //s
        {0,0,0,4,4,62,4,4,4,4,56,0,0,0,0,0}, //t
        {0,0,0,0,0,34,34,34,34,50,44,0,0,0,0,0}, //u
        {0,0,0,0,0,34,34,20,20,20,8,0,0,0,0,0}, //v
        {0,0,0,0,0,34,34,42,42,20,20,0,0,0,0,0}, //w
        {0,0,0,0,0,66,36,24,24,36,66,0,0,0,0,0}, //x
        {0,0,0,0,0,66,66,68,36,40,16,16,8,8,6,0}, //y
        {0,0,0,0,0,62,16,8,4,2,62,0,0,0,0,0}, //z
        {24,4,4,4,4,4,4,4,2,4,4,4,4,4,4,24}, //{
        {0,8,8,8,8,8,8,8,8,8,8,8,8,8,8,0}, //|
        {24,32,32,32,32,32,32,32,64,32,32,32,32,32,32,24}, //}
        {0,0,0,0,0,0,68,76,50,34,0,0,0,0,0,0}} //~
    };
}

void better::setPixel(SDL_Surface* surface, int x, int y, Uint32 pixel, int column, int row, int charWidth, int charHeight) {
    SDL_LockSurface(surface); //use pixelIndex here
    Uint32 rowOffset = row * surface->w * charHeight; //select the chars pixels by getting the row
    Uint32 location = rowOffset + column * charWidth; //then the column
    Uint32* targetPixel = &(((Uint32*)(surface->pixels))[location + x + y * surface->w]); //then from there select the individual pixel to change
    int redComponent {static_cast<int>((pixel & 0xFF000000) >> 24)};
    int greenComponent {static_cast<int>((pixel & 0x00FF0000) >> 16)};
    int blueComponent {static_cast<int>((pixel & 0x0000FF00) >> 8)};
    int alphaComponent {static_cast<int>(pixel & 0x000000FF)};
    *targetPixel = SDL_MapRGBA(surface->format, redComponent, greenComponent, blueComponent, alphaComponent);
    SDL_UnlockSurface(surface);
}

void better::renderLetter(SDL_Surface* surface, Uint8 pixelGrid[12], int column, int row, Uint32 colorfg, Uint32 colorbg) {
    for(int i{}; i < 16; ++i) {
        for(int j{}; j < 8; ++j) {
            better::setPixel(surface, j, i, better::unpackUint8Bit(j + 1, pixelGrid[i], colorfg, colorbg), column, row, 8, 16); //basically set each pixel for a character
        }
    }
}

void better::renderCursor(SDL_Surface* surface, int column, int row, int topLine, int topColumn) {
    Uint8 cursor[16] {
        3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3
    };
    row -= topLine;
    row += 1;
    column -= topColumn;
    for(int i{}; i < 16; ++i) {
        for(int j{}; j < 2; ++j) {
            better::setPixel(surface, j, i, better::unpackUint8Bit(j + 1, cursor[i], 0xAA7069FF, 0x222222FF), column, row, 8, 16);
        }
    }
}

better::charMapArr better::charCheck(char letter, std::array<better::charMapArr,95> letters) {
    return letters[letter - 32];
}

void better::createLetter(SDL_Surface* surface, char letter, int column, int row, Uint32 colorfg, Uint32 colorbg) {
    better::renderLetter(surface, better::charCheck(letter, better::letters).arr, column, row, colorfg, colorbg);
}

void better::renderText(SDL_Surface* surface, immer::flex_vector<immer::flex_vector<char>> vector, int topLine, int topColumn, const int textHeight, const int textWidth, better::Cursor highlightStart, better::Cursor highlightEnd, Uint32 colorbg, Uint32 colorfg, Uint32 colorhighlight, Uint32 colorparens, Uint32 colorcomments) {
    bool highlight {false};
    bool multilineComment {false};
    bool comment {false};
    Uint32 foreground {colorfg};
    Uint32 background {colorbg};
    if(highlightEnd.row != highlightStart.row || highlightEnd.column != highlightStart.column) {
        highlight = true;
    }
    
    int size = vector.size();
    if(vector.size() - 1 >= topLine + textHeight - 1) {
        size = topLine + textHeight - 1;
    }
    for(int i{topLine}, otheri{1}; i < size; ++i, ++otheri) { //TODO: create cases for space, newline and tabs
        int colSize = vector[i].size();
        if(vector[i].size() >= topColumn + textWidth) {
            colSize = topColumn + textWidth;
        }
        comment = false;
        for(int j{topColumn}, otherj{}; j < colSize; ++j, ++otherj) {
            if(highlight && ((i >= highlightStart.row && i <= highlightEnd.row) && (j >= highlightStart.column && j <= highlightEnd.column)) || (((highlightEnd.row != highlightStart.row && highlightEnd.column != highlightStart.column)) && ((i == highlightStart.row && j >= highlightStart.column) || (i == highlightEnd.row && j <= highlightEnd.column) || (i < highlightEnd.row && i > highlightStart.row)))) {
                background = colorhighlight;
            }
            else {
                background = colorbg;
            }
            if(multilineComment && vector[i][j] == '/') {
                if(vector[i][j - 1] == '*') {
                    multilineComment = false;
                    foreground = colorcomments;
                }
            }
            else if(comment || multilineComment) {
                foreground = colorcomments;
            }
            else if(vector[i][j] == '/') {
                foreground = colorparens;
                if(j < vector[i].size() - 1) {
                    if(vector[i][j + 1] == '/') {
                        comment = true;
                        foreground = colorcomments;
                    }
                    else if(vector[i][j + 1] == '*') {
                        multilineComment = true;
                        foreground = colorcomments;
                    }
                } 
            }
            else if(vector[i][j] == '\'' || vector[i][j] == '!' || vector[i][j] == '*' || vector[i][j] == '%' || vector[i][j] == '^' || vector[i][j] == '&' || vector[i][j] == '|' || vector[i][j] == '=' || vector[i][j] == '+' || vector[i][j] == '-' || vector[i][j] == '(' || vector[i][j] == ')' || vector[i][j] == '<' || vector[i][j] == '>'|| vector[i][j] == '[' || vector[i][j] == ']' || vector[i][j] == '{' || vector[i][j] == '}' || vector[i][j] == '\"') {
                foreground = colorparens;
            }
            else {
                foreground = colorfg;
            }
            better::createLetter(surface, vector[i][j], otherj, otheri, foreground, background); //render text line by line
        }
    }
}

Uint32 better::unpackUint8Bit(int index, Uint8 number, Uint32 color, Uint32 colorbg) {
    Uint8 power {1};
    for(int i{1}; i < index; ++i) {
        power *= 2; //find the integer mask to isolate the bit at the correct index, i.e. the first bit (0xb00000001 masked on the number 0xb11010001 will 0 all of the other bits except the last)
    }

    Uint32 result = number & power; //mask the 2 numbers together
    if(result) { //if the number is greater than 0 that bit is set
        result = color;
    }
    else {
        result = colorbg;
    }
    return result;
}

better::Text better::scroll(better::Text text, SDL_Event event, const int textHeight, const int textWidth) { //make sure not to move cursor
    int row {text.topLineNumber + textHeight};
    int column {text.topColumnNumber + textWidth};

    if(event.wheel.y > 0) {
        for(int times{}; times < event.wheel.y; ++times) {
            if(text.topLineNumber) {
                row -= 1;
                text.topLineNumber -= 1;
            }
        }
    }
    else {
        for(int times{}; times > event.wheel.y; --times) {
            if((row == text.topLineNumber + textHeight) && (row - 1 < text.textEdit.size())) {
                row += 1;
                text.topLineNumber += 1;
            }
        }
    }

    if(event.wheel.x > 0) {
        for(int times{}; times < event.wheel.x; ++times) {
            if((column == text.topColumnNumber + textWidth) && (column < text.textEdit[row].size())) {
                column += 1;
                text.topColumnNumber += 1;
            }
        }
    }
    else {
        for(int times{}; times > event.wheel.x; --times) {
            if(text.topColumnNumber) {
                column -= 1;
                text.topColumnNumber -= 1;
            }
        }
    }

    return text;
}
