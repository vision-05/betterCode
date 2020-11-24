#include "renderchars.hpp"

void better::setPixel(SDL_Surface* surface, int x, int y, Uint32 pixel, int column, int row, int charWidth, int charHeight) {
    SDL_LockSurface(surface); //use pixelIndex here
    Uint32 rowOffset = row * surface->w * charHeight; //select the chars pixels by getting the row
    Uint32 location = rowOffset + column * charWidth; //then the column
    Uint32* targetPixel = &(((Uint32*)(surface->pixels))[location + x + y * surface->w]); //then from there select the individual pixel to change
    *targetPixel = pixel;
    SDL_UnlockSurface(surface);
}

void better::renderLetter(SDL_Surface* surface, Uint8 pixelGrid[12], int column, int row) {
    for(int i{}; i < 12; ++i) {
        for(int j{}; j < 8; ++j) {
            better::setPixel(surface, j, i, better::unpackUint8Bit(j + 1,pixelGrid[i]), column, row, 8, 12); //basically set each pixel for a character
        }
    }
}

void better::createLetter(SDL_Surface* surface, char letter, int column, int row) {

    Uint8 cLetter[12] { //give 2 pixel thickness
        0,0,0,0,48,12,2,2,2,12,48,0
    };

    Uint8 CLetter[12] { //give 2 pixel thickness
        0,0,112,12,6,2,2,6,12,112,0,0
    };

    Uint8 iLetter[12] = {
        0,12,12,0,0,12,12,12,12,12,12,0
    };

    Uint8 Iletter[12] = {
        0,12,12,12,12,12,12,12,12,12,12,0
    };

    Uint8 rightArrowLetter[12] = {
        0,0,1,6,24,96,96,24,6,1,0,0
    };

    Uint8 leftArrowLetter[12] = {
        0,0,96,24,6,1,1,6,24,96,0,0
    };

    Uint8 defaultGrid[12] = {
        255,255,255,255,255,255,255,255,255,255,255,255
    };

    switch(letter) {
        case 'i':
            better::renderLetter(surface, iLetter, column, row);
            break;
        case 'I':
            better::renderLetter(surface, Iletter, column, row);
            break;
        case '>':
            better::renderLetter(surface, rightArrowLetter, column, row);
            break;
        case '<':
            better::renderLetter(surface, leftArrowLetter, column, row);
            break;
        case 'C':
            better::renderLetter(surface, CLetter, column, row);
            break;
        case 'c':
            better::renderLetter(surface, cLetter, column, row);
            break;
        default:
            better::renderLetter(surface, defaultGrid, column, row);        
    }

    
}

void better::renderText(SDL_Surface* surface, immer::flex_vector<immer::flex_vector<char>> vector) {
    for(int i{}; i < vector.size(); ++i) { //TODO: create cases for space, newline and tabs
        for(int j{}; j < vector[i].size(); ++ j) {
            better::createLetter(surface, vector[i][j], j, i); //render text line by line
        }
    }
}

Uint32 better::unpackUint8Bit(int index, Uint8 number) {
    Uint8 power {1};
    for(int i{1}; i < index; ++i) {
        power *= 2; //find the integer mask to isolate the bit at the correct index, i.e. the first bit (0xb00000001 masked on the number 0xb11010001 will 0 all of the other bits except the last)
    }

    Uint32 result = number & power; //mask the 2 numbers together
    if(result) { //if the number is greater than 0 that bit is set
        result = 255;
    }
    return result; //else the result is already 0 so no case for that needed
}