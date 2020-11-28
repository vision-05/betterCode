#include "renderchars.hpp"
#include <iostream>

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

void better::renderLetter(SDL_Surface* surface, Uint8 pixelGrid[12], int column, int row) {
    for(int i{}; i < 12; ++i) {
        for(int j{}; j < 8; ++j) {
            better::setPixel(surface, j, i, better::unpackUint8Bit(j + 1, pixelGrid[i], 0x0044AAFF), column, row, 8, 12); //basically set each pixel for a character
        }
    }
}

void better::renderCursor(SDL_Surface* surface, int column, int row, int topLine, int topColumn) {
    Uint8 cursor[12] {
        3,3,3,3,3,3,3,3,3,3,3,3
    };
    row -= topLine;
    column -= topColumn;
    for(int i{}; i < 12; ++i) {
        for(int j{}; j < 2; ++j) {
            better::setPixel(surface, j, i, better::unpackUint8Bit(j + 1, cursor[i], 0x0044AAFF), column, row, 8, 12);
        }
    }
}

better::charMapArr better::charCheck(char letter) {
    int ascii {static_cast<int>(letter)};

    if(ascii > 64 && ascii < 91) { //letters (uppercase)
        Uint8 uppercase[26][12] {
            {0,0,0,0,0,0,0,0,0,0,0,0}, //A
            {0,0,0,0,0,0,0,0,0,0,0,0}, //B
            {0,0,112,12,6,2,2,6,12,112,0,0}, //C
            {0,0,0,0,0,0,0,0,0,0,0,0}, //D
            {0,0,0,0,0,0,0,0,0,0,0,0}, //E
            {0,0,0,0,0,0,0,0,0,0,0,0}, //F
            {0,0,0,0,0,0,0,0,0,0,0,0}, //G
            {0,0,0,0,0,0,0,0,0,0,0,0}, //H
            {0,8,8,8,8,8,8,8,8,8,8,0}, //I
            {0,0,0,0,0,0,0,0,0,0,0,0}, //J
            {0,0,0,0,0,0,0,0,0,0,0,0}, //K
            {0,0,0,0,0,0,0,0,0,0,0,0}, //L
            {0,0,0,0,0,0,0,0,0,0,0,0}, //M
            {0,0,0,0,0,0,0,0,0,0,0,0}, //N
            {0,0,0,0,0,0,0,0,0,0,0,0}, //O
            {0,0,0,0,0,0,0,0,0,0,0,0}, //P
            {0,0,0,0,0,0,0,0,0,0,0,0}, //Q
            {0,0,0,0,0,0,0,0,0,0,0,0}, //R
            {0,0,0,0,0,0,0,0,0,0,0,0}, //S
            {0,0,0,0,0,0,0,0,0,0,0,0}, //T
            {0,0,0,0,0,0,0,0,0,0,0,0}, //U
            {0,0,0,0,0,0,0,0,0,0,0,0}, //V
            {0,0,0,0,0,0,0,0,0,0,0,0}, //W
            {0,0,0,0,0,0,0,0,0,0,0,0}, //X
            {0,0,0,0,0,0,0,0,0,0,0,0}, //Y
            {0,0,0,0,0,0,0,0,0,0,0,0} //Z
        };
        ascii -= 65;
        return better::makeCharMapArr(uppercase[ascii]);
    }
    else if(ascii > 96 && ascii < 123) { //letters (lowercase)
        Uint8 letters[26][12] {
            {0,0,0,0,0,12,10,9,25,21,22,0}, //a
            {0,1,1,1,1,1,1,15,17,17,15,0}, //b
            {0,0,0,0,48,12,2,2,2,12,48,0}, //c
            {0,16,16,16,16,16,30,19,17,19,31,0}, //d
            {0,0,0,0,12,34,30,2,4,4,24,0}, //e
            {0,48,8,4,30,4,4,4,4,4,4,0}, //f
            {0,0,0,8,20,20,28,16,16,18,12,0}, //g
            {0,1,1,1,1,1,13,27,33,33,33,0}, //h
            {0,8,8,0,0,8,8,8,8,8,8,0}, //i
            {0,0,0,0,0,0,0,0,0,0,0,0}, //j
            {0,0,0,0,0,0,0,0,0,0,0,0}, //k
            {0,0,0,0,0,0,0,0,0,0,0,0}, //l
            {0,0,0,0,0,0,0,0,0,0,0,0}, //m
            {0,0,0,0,0,0,0,0,0,0,0,0}, //n
            {0,0,0,0,0,0,0,0,0,0,0,0}, //o
            {0,0,0,0,0,0,0,0,0,0,0,0}, //p
            {0,0,0,0,0,0,0,0,0,0,0,0}, //q
            {0,0,0,0,0,0,0,0,0,0,0,0}, //r
            {0,0,0,0,0,0,0,0,0,0,0,0}, //s
            {0,0,0,0,0,0,0,0,0,0,0,0}, //t
            {0,0,0,0,0,0,0,0,0,0,0,0}, //u
            {0,0,0,0,0,0,0,0,0,0,0,0}, //v
            {0,0,0,0,0,0,0,0,0,0,0,0}, //w
            {0,0,0,0,0,0,0,0,0,0,0,0}, //x
            {0,0,0,0,0,0,0,0,0,0,0,0}, //y
            {0,0,0,0,0,0,0,0,0,0,0,0} //z
        };
        ascii -= 97;
        return better::makeCharMapArr(letters[ascii]);
    }
    else if(ascii > 31 && ascii < 48) { //first symbols
        Uint8 symbols1[16][12] {
            {0,0,0,0,0,0,0,0,0,0,0,0}, //Space
            {0,0,0,0,0,0,0,0,0,0,0,0}, //!
            {0,0,0,0,0,0,0,0,0,0,0,0}, //"
            {0,54,54,127,127,54,54,127,127,54,54,0}, //#
            {0,0,0,0,0,0,0,0,0,0,0,0}, //$
            {0,0,0,0,0,0,0,0,0,0,0,0}, //%
            {0,0,0,0,0,0,0,0,0,0,0,0}, //&
            {0,0,0,0,0,0,0,0,0,0,0,0}, //'
            {0,0,0,0,0,0,0,0,0,0,0,0}, //(
            {0,0,0,0,0,0,0,0,0,0,0,0}, //)
            {0,0,0,0,0,0,0,0,0,0,0,0}, //*
            {0,0,0,0,0,0,0,0,0,0,0,0}, //+
            {0,0,0,0,0,0,0,0,0,0,0,0}, //,
            {0,0,0,0,0,0,0,0,0,0,0,0}, //-
            {0,0,0,0,0,0,0,0,0,0,0,0}, //.
            {0,0,0,0,0,0,0,0,0,0,0,0} ///
        };
        ascii -= 32;
        return better::makeCharMapArr(symbols1[ascii]);
    }
    else if(ascii > 47 && ascii < 58) { //numbers
        Uint8 numbers[10][12] {
            {0,0,0,0,0,0,0,0,0,0,0,0}, //0
            {0,0,0,0,0,0,0,0,0,0,0,0}, //1
            {0,0,0,0,0,0,0,0,0,0,0,0}, //2
            {0,0,0,0,0,0,0,0,0,0,0,0}, //3
            {0,0,0,0,0,0,0,0,0,0,0,0}, //4
            {0,0,0,0,0,0,0,0,0,0,0,0}, //5
            {0,0,0,0,0,0,0,0,0,0,0,0}, //6
            {0,0,0,0,0,0,0,0,0,0,0,0}, //7
            {0,0,0,0,0,0,0,0,0,0,0,0}, //8
            {0,0,0,0,0,0,0,0,0,0,0,0} //9
        };
        ascii -= 48;
        return better::makeCharMapArr(numbers[ascii]);
    }
    else if(ascii > 57 && ascii < 64) { //second symbols
        Uint8 symbols2[7][12] {
            {0,0,0,0,0,0,0,0,0,0,0,0}, //:
            {0,0,0,0,0,0,0,0,0,0,0,0}, //;
            {0,0,96,24,6,1,1,6,24,96,0,0}, //<
            {0,0,0,0,0,0,0,0,0,0,0,0}, //=
            {0,0,1,6,24,96,96,24,6,1,0,0}, //>
            {0,0,0,0,0,0,0,0,0,0,0,0}, //?
            {0,0,0,0,0,0,0,0,0,0,0,0} //@
        };
        ascii -= 58;
        return better::makeCharMapArr(symbols2[ascii]);
    }
    else if(ascii > 90 && ascii < 97) { //third symbols
        Uint8 symbols3[6][12] {
            {0,0,0,0,0,0,0,0,0,0,0,0}, //[
            {0,0,0,0,0,0,0,0,0,0,0,0}, //Backslash
            {0,0,0,0,0,0,0,0,0,0,0,0}, //]
            {0,0,0,0,0,0,0,0,0,0,0,0}, //^
            {0,0,0,0,0,0,0,0,0,0,0,0}, //_
            {0,0,0,0,0,0,0,0,0,0,0,0} //`
        };
        ascii -= 91;
        return better::makeCharMapArr(symbols3[ascii]);
    }
    else if(ascii > 122 && ascii < 127) { //fourth symbols
        Uint8 symbols4[4][12] {
            {0,0,0,0,0,0,0,0,0,0,0,0}, //{
            {0,0,0,0,0,0,0,0,0,0,0,0}, //|
            {0,0,0,0,0,0,0,0,0,0,0,0}, //}
            {0,0,0,0,0,0,0,0,0,0,0,0} //~
        };
        ascii -= 123;
        return better::makeCharMapArr(symbols4[ascii]);
    }
    else {
        Uint8 unknown[12] {
            255,255,255,255,255,255,255,255,255,255,255,255
        };
        return better::makeCharMapArr(unknown);
    }
}

void better::createLetter(SDL_Surface* surface, char letter, int column, int row) {
    better::renderLetter(surface, better::charCheck(letter).arr, column, row);
}

void better::renderText(SDL_Surface* surface, immer::flex_vector<immer::flex_vector<char>> vector, int topLine, int topColumn) {
    int size = vector.size();
    if(vector.size() - 1 >= topLine + 66) {
        size = topLine + 66;
    }
    for(int i{topLine}, otheri{}; i < size; ++i, ++otheri) { //TODO: create cases for space, newline and tabs
        int colSize = vector[i].size();
        if(vector[i].size() >= topColumn + 150) {
            colSize = topColumn + 150;
        }
        for(int j{topColumn}, otherj{}; j < colSize; ++j, ++otherj) {
            better::createLetter(surface, vector[i][j], otherj, otheri); //render text line by line
        }
    }
}

Uint32 better::unpackUint8Bit(int index, Uint8 number, Uint32 color) {
    Uint8 power {1};
    for(int i{1}; i < index; ++i) {
        power *= 2; //find the integer mask to isolate the bit at the correct index, i.e. the first bit (0xb00000001 masked on the number 0xb11010001 will 0 all of the other bits except the last)
    }

    Uint32 result = number & power; //mask the 2 numbers together
    if(result) { //if the number is greater than 0 that bit is set
        result = color;
    }
    else {
        result = 0x000000FF;
    }
    return result;
}