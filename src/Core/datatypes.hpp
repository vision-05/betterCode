//! @file

#ifndef DATATYPES_HPP
#define DATATYPES_HPP

#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <immer-0.6.2/immer/flex_vector.hpp>

namespace better {

struct Cursor { //store the line number and column number of the cursor
    int row;
    int column;
};

better::Cursor findCursorPos(int topLine, int topColumn, SDL_Event event, int columnOffset, int characterHeight, int characterWidth);

struct editorData {
    bool menusToDraw[4] = {false, false, false, false};
    bool isShift {false};
    bool isCaps {false};
    bool isScroll {false};
    bool isCtrl {false};
    bool clearHistory {false};
    int switchEditor {0};
    int index {-1};
    std::vector<std::string> menu;
    std::string filename;
    int textHeight;
    int textWidth;
};

struct Text {
    immer::flex_vector<immer::flex_vector<char>> textEdit; //keeps the text state
    better::Cursor cursor; //keeps the cursor state
    better::editorData data;
    int topLineNumber; //keeps top line number so can calculate new line being clicked on
    int topColumnNumber;
    better::Cursor highlightStart {0,0};
    better::Cursor highlightEnd {0,0};
};

struct ConfigData {
    Uint32 foregroundColor;
    Uint32 backgroundColor;
    Uint32 identifierColor;
    Uint32 keywordColor;
    Uint32 symbolColor;
    Uint32 cursorColor;
    Uint32 lineNoColor;
    Uint32 highlightColor;
    Uint32 commentColor;
    Uint32 lineSelectColor;
    Uint32 menuColor;
    Uint32 menuBarColor;
    Uint32 menuTextColor;
    int characterHeight;
    int characterWidth;
};

better::Text updateText(better::Text textEdit, char newChar);
better::Text backspace(better::Text textEdit);
better::Text newLine(better::Text textEdit, bool autoIndent);
better::Text tab(better::Text text);

Uint8 getRed(Uint32 color);
Uint8 getGreen(Uint32 color);
Uint8 getBlue(Uint32 color);
Uint8 getAlpha(Uint32 color);

int getPreviousIndentLevel(better::Text text, int row);

}

#endif
