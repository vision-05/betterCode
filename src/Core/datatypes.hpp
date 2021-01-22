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

better::Cursor findCursorPos(int topLine, int topColumn, SDL_Event event, int columnOffset);

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

struct charMapArr {
    ::Uint8 arr[16];
};

better::charMapArr makeCharMapArr(::Uint8 charArray[16]); 

better::Text updateText(better::Text textEdit, char newChar);
better::Text backspace(better::Text textEdit);
better::Text newLine(better::Text textEdit, bool autoIndent);
better::Text tab(better::Text text);

Uint8 getRed(Uint32 color);
Uint8 getGreen(Uint32 color);
Uint8 getBlue(Uint32 color);
Uint8 getAlpha(Uint32 color);

better::Text scroll(better::Text text, SDL_Event event);
better::Text scrollUp(better::Text text, int y, int row);
better::Text scrollDown(better::Text text, int y, int row);
better::Text scrollLeft(better::Text text, int x, int column);
better::Text scrollRight(better::Text text, int x, int column, int row);

int getPreviousIndentLevel(better::Text text, int row);

}

#endif
