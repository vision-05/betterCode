#include "renderchars.hpp"
//TODO: symbols have different colours to text

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
            better::setPixel(surface, j, i, better::unpackUint8Bit(j + 1, pixelGrid[i], 0x5588AAFF), column, row, 8, 12); //basically set each pixel for a character
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
            better::setPixel(surface, j, i, better::unpackUint8Bit(j + 1, cursor[i], 0xAA7069FF), column, row, 8, 12);
        }
    }
}

better::charMapArr better::charCheck(char letter) {
    int ascii {static_cast<int>(letter)};

    if(ascii > 64 && ascii < 91) { //letters (uppercase): done
        Uint8 uppercase[26][12] {
            {0,0,24,24,36,36,90,66,66,66,66,0}, //A
            {0,0,14,18,18,14,18,34,34,18,14,0}, //B
            {0,0,112,12,6,2,2,2,6,12,112,0}, //C
            {0,0,6,10,18,34,34,34,18,10,6,0}, //D
            {0,0,62,2,2,2,30,2,2,2,62,0}, //E
            {0,0,62,2,2,2,62,2,2,2,2,0}, //F
            {0,0,56,4,2,2,58,34,34,36,56,0}, //G
            {0,0,34,34,34,34,62,34,34,34,34,0}, //H
            {0,0,28,8,8,8,8,8,8,8,28,0}, //I
            {0,0,62,32,32,32,32,32,34,20,8,0}, //J
            {0,0,18,18,10,10,2,10,10,18,18,0}, //K
            {0,0,2,2,2,2,2,2,2,2,62,0}, //L
            {0,0,66,66,102,102,90,66,66,66,66,0}, //M
            {0,0,34,34,38,38,42,42,50,50,34,0}, //N
            {0,0,24,36,66,66,66,66,66,36,24,0}, //O
            {0,0,6,14,18,18,18,14,2,2,2,0}, //P
            {0,0,24,36,66,66,66,66,66,36,24,112}, //Q
            {0,0,14,18,18,18,14,2,6,10,18,0}, //R
            {0,0,120,4,2,4,24,32,64,64,62,0}, //S
            {0,0,62,8,8,8,8,8,8,8,8,0}, //T
            {0,0,66,66,66,66,66,66,66,36,24,0}, //U
            {0,0,66,66,66,66,36,36,36,24,24,0}, //V
            {0,0,66,66,66,66,90,102,102,66,66,0}, //W
            {0,0,66,66,36,36,24,36,36,66,66,0}, //X
            {0,0,34,34,20,20,8,8,8,8,8,0}, //Y
            {0,0,126,64,32,32,16,8,8,4,126,0} //Z
        };
        ascii -= 65;
        return better::makeCharMapArr(uppercase[ascii]);
    }
    else if(ascii > 96 && ascii < 123) { //letters (lowercase): done
        Uint8 letters[26][12] {
            {0,0,0,0,0,12,10,9,25,21,22,0}, //a
            {0,2,2,2,2,2,2,30,18,18,30,0}, //b
            {0,0,0,0,48,12,4,2,4,12,48,0}, //c
            {0,16,16,16,16,16,30,19,17,19,30,0}, //d
            {0,0,0,0,12,34,30,2,4,4,24,0}, //e
            {0,48,8,4,30,4,4,4,4,4,4,0}, //f
            {0,0,0,8,20,20,28,16,16,18,12,0}, //g
            {0,2,2,2,2,2,14,30,34,34,34,0}, //h
            {0,8,8,0,0,8,8,8,8,8,8,0}, //i
            {0,8,8,0,8,8,8,8,8,10,4,0}, //j
            {0,0,2,18,10,6,2,6,10,18,18,0}, //k
            {0,14,8,8,8,8,8,8,8,8,62,0}, //l
            {0,0,0,0,0,36,90,90,90,90,90,0}, //m
            {0,0,0,0,0,14,18,18,18,18,18,0}, //n
            {0,0,0,0,0,24,36,66,66,36,24,0}, //o
            {0,0,0,12,18,18,6,2,2,2,2,0}, //p
            {0,0,0,12,18,18,10,16,16,16,16,0}, //q
            {0,0,0,0,0,26,38,2,2,2,2,0}, //r
            {0,0,0,0,60,2,2,28,32,32,30,0}, //s
            {0,0,0,4,4,62,4,4,36,36,24,0}, //t
            {0,0,0,0,0,34,34,34,50,52,48,0}, //u
            {0,0,0,0,0,66,66,36,36,24,24,0}, //v
            {0,0,0,0,0,130,130,68,84,40,40,0}, //w
            {0,0,0,0,0,66,36,24,24,36,66,0}, //x
            {0,0,0,66,36,40,16,16,8,10,4,0}, //y
            {0,0,0,0,0,62,16,8,4,2,62,0} //z
        };
        ascii -= 97;
        return better::makeCharMapArr(letters[ascii]);
    }
    else if(ascii > 31 && ascii < 48) { //first symbols: 3 more
        Uint8 symbols1[16][12] {
            {0,0,0,0,0,0,0,0,0,0,0,0}, //Space
            {0,8,8,8,8,8,8,0,0,8,8,0}, //!
            {0,20,20,20,0,0,0,0,0,0,0,0}, //"
            {0,54,54,127,127,54,54,127,127,54,54,0}, //# make thinner
            {0,0,0,0,0,0,0,0,0,0,0,0}, //$ not done
            {0,0,0,0,0,0,0,0,0,0,0,0}, //% not done
            {0,0,0,0,0,0,0,0,0,0,0,0}, //& not done
            {0,8,8,8,0,0,0,0,0,0,0,0}, //'
            {16,16,8,8,4,4,4,4,8,8,16,16}, //(
            {4,4,8,8,16,16,16,16,8,8,4,4}, //)
            {0,42,20,62,20,42,0,0,0,0,0,0}, //*
            {0,0,0,0,0,8,8,62,8,8,0,0}, //+
            {0,0,0,0,0,0,0,0,8,8,4,0}, //,
            {0,0,0,0,0,0,62,0,0,0,0,0}, //-
            {0,0,0,0,0,0,0,0,0,8,8,0}, //.
            {0,64,32,32,16,16,8,8,4,4,2,0} ///
        };
        ascii -= 32;
        return better::makeCharMapArr(symbols1[ascii]);
    }
    else if(ascii > 47 && ascii < 58) { //numbers: 5 more
        Uint8 numbers[10][12] {
            {0,24,36,36,66,114,78,66,36,36,24,0}, //0
            {0,16,24,20,16,16,16,16,16,16,60,0}, //1
            {0,24,36,68,66,64,32,32,16,12,126,0}, //2
            {0,14,48,32,32,28,16,32,32,48,14,0}, //3
            {0,32,48,48,40,40,36,126,32,32,32,0}, //4
            {0,0,0,0,0,0,0,0,0,0,0,0}, //5 not done
            {0,0,0,0,0,0,0,0,0,0,0,0}, //6 not done
            {0,0,0,0,0,0,0,0,0,0,0,0}, //7 not done
            {0,0,0,0,0,0,0,0,0,0,0,0}, //8 not done
            {0,0,0,0,0,0,0,0,0,0,0,0} //9 not done
        };
        ascii -= 48;
        return better::makeCharMapArr(numbers[ascii]);
    }
    else if(ascii > 57 && ascii < 64) { //second symbols: 2 more
        Uint8 symbols2[7][12] {
            {0,0,0,8,8,0,0,0,8,8,0,0}, //:
            {0,0,0,8,8,0,0,0,8,8,4,0}, //;
            {0,0,96,24,6,1,1,6,24,96,0,0}, //<
            {0,0,0,0,0,126,0,126,0,0,0,0}, //=
            {0,0,1,6,24,96,96,24,6,1,0,0}, //>
            {0,0,0,0,0,0,0,0,0,0,0,0}, //? not done
            {0,0,0,0,0,0,0,0,0,0,0,0} //@ not done
        };
        ascii -= 58;
        return better::makeCharMapArr(symbols2[ascii]);
    }
    else if(ascii > 90 && ascii < 97) { //third symbols: done
        Uint8 symbols3[6][12] {
            {28,4,4,4,4,4,4,4,4,4,4,28}, //[
            {0,2,4,4,8,8,16,16,32,32,64,0}, //Backslash
            {56,32,32,32,32,32,32,32,32,32,32,56}, //]
            {0,8,20,34,0,0,0,0,0,0,0,0}, //^
            {0,0,0,0,0,0,0,0,0,0,127,0}, //_
            {0,6,8,0,0,0,0,0,0,0,0,0} //`
        };
        ascii -= 91;
        return better::makeCharMapArr(symbols3[ascii]);
    }
    else if(ascii > 122 && ascii < 127) { //fourth symbols: 3 more
        Uint8 symbols4[4][12] {
            {0,0,0,0,0,0,0,0,0,0,0,0}, //{
            {0,8,8,8,8,8,8,8,8,8,8,0}, //|
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
        result = 0x222222FF;
    }
    return result;
}