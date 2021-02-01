//! @file
//! This file handles all of the text rendering functions

#include "renderchars.hpp"
//TODO: symbols have different colours to text

namespace better {
    static std::array<std::vector<Uint8>, 256> letters {
        {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //0
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //1
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //2
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //3
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //4
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //5
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //6
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //7
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //8
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //9
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //10
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //11
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //12
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //13
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //14
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //15
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //16
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //17
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //18
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //19
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //20
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //21
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //22
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //23
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //24
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //25
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //26
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //27
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //28
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //29
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //30
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //31
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //Space
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
        {0,0,0,0,0,0,68,76,50,34,0,0,0,0,0,0} //~
        
        }
    };
}

void better::setPixel(SDL_Surface* surface, int x, int y, Uint32 pixel, int column, int row, int charWidth, int charHeight) {
    SDL_LockSurface(surface); //use pixelIndex here
    Uint32 rowOffset = row * surface->w * charHeight; //select the chars pixels by getting the row
    Uint32 location = rowOffset + column * charWidth; //then the column
    Uint32* targetPixel = &(reinterpret_cast<Uint32*>(surface->pixels)[location + x + y * surface->w]); //then from there select the individual pixel to change
    int redComponent {static_cast<int>((pixel & 0xFF000000) >> 24)};
    int greenComponent {static_cast<int>((pixel & 0x00FF0000) >> 16)};
    int blueComponent {static_cast<int>((pixel & 0x0000FF00) >> 8)};
    int alphaComponent {static_cast<int>(pixel & 0x000000FF)};
    *targetPixel = SDL_MapRGBA(surface->format, redComponent, greenComponent, blueComponent, alphaComponent);
    SDL_UnlockSurface(surface);
}

void better::renderLetter(SDL_Surface* surface, std::vector<Uint8> pixelGrid, int column, int row, Uint32 colorfg, Uint32 colorbg, int characterHeight, int characterWidth) {
    for(int i{}; i < characterHeight; ++i) {
        for(int j{}; j < characterWidth; ++j) {
            better::setPixel(surface, j, i, better::unpackUint8Bit(j + 1, pixelGrid[i], colorfg, colorbg), column, row, characterWidth, characterHeight); //basically set each pixel for a character
        }
    }
}

void better::renderCursor(SDL_Surface* surface, better::Cursor cursor, better::ConfigData config, int topLine, int topColumn, int columnOffset) {
    Uint8 caret[16] {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3};
    cursor.row -= topLine;
    cursor.row += 1;
    cursor.column -= topColumn;
    for(int i{}; i < config.characterHeight; ++i) {
        for(int j{}; j < 2; ++j) {
            better::setPixel(surface, j, i, better::unpackUint8Bit(j + 1, caret[i], 0xAA7069FF, 0x222222FF), cursor.column + static_cast<int>(columnOffset / config.characterWidth), cursor.row, config.characterWidth, config.characterHeight);
        }
    }
}

std::vector<Uint8> better::charCheck(char letter, std::array<std::vector<Uint8>,256> letters) {
    return letters[letter];
}

void better::renderText(SDL_Surface* surface, better::Text text, better::ConfigData config, int columnOffset, better::Text previousText, bool firstRender) {
    bool highlight {false};
    bool multilineComment {false};
    bool comment {false};
    Uint32 foreground {config.foregroundColor};
    Uint32 background {config.backgroundColor};
    if(text.highlightEnd.row != text.highlightStart.row || text.highlightEnd.column != text.highlightStart.column) {
        highlight = true;
    }
    
    int size {static_cast<int>(text.textEdit.size())};
    if(size - 1 >= text.topLineNumber + text.data.textHeight - 1) {
        size = text.topLineNumber + text.data.textHeight - 1;
    }
    int letterCount {0};
    for(int lineIndex{text.topLineNumber}, rowIndex{1}; lineIndex < size; ++lineIndex, ++rowIndex) { //TODO: create cases for space, newline and tabs
        int colSize = text.textEdit[lineIndex].size();
        if(text.textEdit[lineIndex].size() >= text.topColumnNumber + text.data.textWidth) {
            colSize = text.topColumnNumber + text.data.textWidth;
        }
        comment = false;
        SDL_Rect line {.x = columnOffset, .y = rowIndex * config.characterHeight, .w = text.data.textWidth * config.characterWidth, .h = config.characterHeight};
        if(lineIndex != previousText.cursor.row && !firstRender && previousText.textEdit[lineIndex] == text.textEdit[lineIndex] && text.topLineNumber == previousText.topLineNumber && text.topColumnNumber == previousText.topColumnNumber) {
            continue;
        }
        SDL_FillRect(surface, &line, SDL_MapRGB(surface->format, better::getRed(config.backgroundColor), better::getGreen(config.backgroundColor), better::getBlue(config.backgroundColor)));
        for(int letterIndex{}, columnIndex{}; letterIndex < colSize; ++letterIndex, ++columnIndex) {
            background = getBackground(text, config, lineIndex, letterIndex);
            foreground = getForeground(text, config, lineIndex, letterIndex, comment, multilineComment);
            
            
            if(letterIndex >= text.topColumnNumber && letterIndex < text.topColumnNumber + text.data.textWidth - 1) {
                ++letterCount;
                auto letterStart {std::chrono::steady_clock::now()};
                better::renderLetter(surface, better::charCheck(text.textEdit[lineIndex][letterIndex],better::letters), columnIndex - text.topColumnNumber + static_cast<int>(columnOffset / config.characterWidth), rowIndex, foreground, background, config.characterHeight, config.characterWidth); //render text line by line
                auto letterEnd {std::chrono::steady_clock::now()};
                std::chrono::duration<double> letterDur {letterEnd - letterStart};
                std::cout << "Letter: " << letterDur.count() << '\n';
                std::cout << "Count: " << letterCount << '\n';
            }
        }
    }  
}

Uint32 better::getBackground(better::Text text, better::ConfigData config, int lineIndex, int letterIndex) {
    better::Cursor currentLetter {lineIndex, letterIndex};
    if(
        (currentLetter >= text.highlightStart && currentLetter <= text.highlightEnd) || 
        (
            (
                (text.highlightEnd.row != text.highlightStart.row && text.highlightEnd.column != text.highlightStart.column)
            ) && 
            (
                (lineIndex == text.highlightStart.row && letterIndex >= text.highlightStart.column) ||
                (lineIndex == text.highlightEnd.row && letterIndex <= text.highlightEnd.column) ||
                (lineIndex < text.highlightEnd.row && lineIndex > text.highlightStart.row)
            )
        )
    ) {
        return config.highlightColor;
    }
    return config.backgroundColor;
}

Uint32 better::getForeground(better::Text text, better::ConfigData config, int lineIndex, int letterIndex, bool& isComment, bool& isMultilineComment) {
    if(isMultilineComment && text.textEdit[lineIndex][letterIndex] == '/') {
        if(text.textEdit[lineIndex][letterIndex - 1] == '*') {
            isMultilineComment = false;
            return config.commentColor;
        }
    }
    else if(isComment || isMultilineComment) {
        return config.commentColor;
    }
    else if(text.textEdit[lineIndex][letterIndex] == '/') {
        if(letterIndex < text.textEdit[lineIndex].size() - 1) {
            if(text.textEdit[lineIndex][letterIndex + 1] == '/') {
                isComment = true;
                return config.commentColor;
            }
            else if(text.textEdit[lineIndex][letterIndex + 1] == '*') {
                isMultilineComment = true;
                return config.commentColor;
            }
        }
        return config.symbolColor;
    }
    else if((text.textEdit[lineIndex][letterIndex] > 32 && text.textEdit[lineIndex][letterIndex] < 48) ||
    (text.textEdit[lineIndex][letterIndex] > 59 && text.textEdit[lineIndex][letterIndex] < 63) ||
    (text.textEdit[lineIndex][letterIndex] > 90 && text.textEdit[lineIndex][letterIndex] < 95) ||
    (text.textEdit[lineIndex][letterIndex] > 122 && text.textEdit[lineIndex][letterIndex] < 126)) {
        return config.symbolColor;
    }
    else {
        return config.foregroundColor;
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



void better::renderLineNumbers(SDL_Surface* surface, better::ConfigData config, better::editorData data, int topLine, int columnOffset, int textLength, int editorHeight) {
    std::string number {std::to_string(textLength)};
    SDL_Rect side {.x = columnOffset, .y = config.characterHeight, .w = static_cast<int>(number.size() + 1) * config.characterWidth, .h = editorHeight};
    SDL_FillRect(surface, &side, SDL_MapRGBA(surface->format, better::getRed(config.backgroundColor), better::getGreen(config.backgroundColor), better::getBlue(config.backgroundColor), better::getAlpha(config.backgroundColor)));
    if(editorHeight > textLength) {
        editorHeight = textLength + 1;
    }
    for(int i{topLine + 1}; i < topLine + data.textHeight; ++i) {
        number = std::to_string(i);
        number.push_back(' ');
        for(int j{}; j < number.size(); ++j) {
            better::renderLetter(surface, better::charCheck(number[j], better::letters), j + static_cast<int>(columnOffset / config.characterWidth), i - topLine, config.foregroundColor, config.backgroundColor, config.characterHeight, config.characterWidth);
        }
    }
}
