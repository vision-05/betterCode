//! @file
//! This file contains all of the user defined structs used throughout the project
#include "datatypes.hpp"

better::Text better::updateText(better::Text textEdit, char newChar) {
    int textHeight {textEdit.data.textHeight};
    int textWidth {textEdit.data.textWidth};
    //change the topline number variable if needs to scroll
    bool endOfLine {textEdit.cursor.column == textEdit.textEdit[textEdit.cursor.row].size()};
    bool endOfScreen {textEdit.cursor.column + 1 >= textEdit.topColumnNumber + textWidth};
    immer::flex_vector<char> line {endOfLine ? textEdit.textEdit[textEdit.cursor.row].push_back(newChar) : textEdit.textEdit[textEdit.cursor.row].insert(textEdit.cursor.column, newChar)}; //add the character to the line (change to insert unless the cursor is at the end of the line)
    better::Text tempText
    {
        textEdit.textEdit.set(textEdit.cursor.row,line), //textEdit
        {textEdit.cursor.row, textEdit.cursor.column + 1}, //cursor
        { //data start
            {false,false,false,false}, //menusToDraw
            textEdit.data.isShift, //isShift
            textEdit.data.isCaps, //isCaps
            textEdit.data.isScroll, //isScroll
            textEdit.data.isCtrl, //isCtrl
            textEdit.data.clearHistory, //clearHistory
            textEdit.data.switchEditor, //switchEditor
            -1, //index
            textEdit.data.menu, //menu
            textEdit.data.filename, //filename
            textEdit.data.textHeight, //textHeight
            textEdit.data.textWidth //textWidth
        }, //data end
        textEdit.topLineNumber, //topLineNumber
        endOfScreen ? textEdit.topColumnNumber + 1 : textEdit.topColumnNumber, //topColumnNumber
        textEdit.highlightStart, //hightlightStart
        textEdit.highlightEnd //higlightEnd
    };
    return tempText;
}

better::Text better::backspace(better::Text text) { //couple of bugs with backspace, figure out once got all chars rendering
    int textHeight {text.data.textHeight};
    int textWidth {text.data.textWidth};
    if(!text.cursor.row && !text.cursor.column) {
        return text;
    }

    immer::flex_vector<char> newLine;

    if(text.cursor.column > 0) {
        if(text.topColumnNumber == text.cursor.column) {
            text.topColumnNumber -= 1; 
        }
        newLine = text.textEdit[text.cursor.row].erase(text.cursor.column - 1); //make case for deleting newline
    }
    else {
        //concatenate rest of line with previous line
        if(text.textEdit[text.cursor.row].size() > 0) {
            int colNo = text.textEdit[text.cursor.row - 1].size();
            int topColumnNo {text.topColumnNumber};
            if(colNo > textWidth) {
                topColumnNo = colNo - textWidth;
            }
            newLine = text.textEdit[text.cursor.row - 1] + text.textEdit[text.cursor.row];
            return
            { //Text start
                text.textEdit.erase(text.cursor.row).set(text.cursor.row - 1, newLine), //textEdit
                {text.cursor.row - 1, colNo}, //cursor
                { //data start
                    {false,false,false,false}, //menusToDraw
                    text.data.isShift, //isShift
                    text.data.isCaps, //isCaps
                    text.data.isScroll, //isScroll
                    text.data.isCtrl, //isCtrl
                    text.data.clearHistory, //clearHistory
                    text.data.switchEditor, //switchEditor
                    -1, //index
                    text.data.menu, //menu
                    text.data.filename, //filename
                    text.data.textHeight, //textHeight
                    text.data.textWidth //textWidth
                }, //data end
                text.cursor.row >= text.textEdit.size() - 1 ? text.topLineNumber - 1 : text.topLineNumber, //topLineNumber
                topColumnNo, //topColumnNumber
                text.highlightStart, //highlightStart
                text.highlightEnd //highlightEnd
            }; //Text end
        }
        return
        { //Text start
            text.textEdit.erase(text.cursor.row), //textEdit
            {text.cursor.row - 1, static_cast<int>(text.textEdit[text.cursor.row - 1].size())}, //cursor
            { //data start
                {false,false,false,false}, //menusToDraw
                text.data.isShift, //isShift
                text.data.isCaps, //isCaps
                text.data.isScroll, //isScroll
                text.data.isCtrl, //isCtrl
                text.data.clearHistory, //clearHistory
                text.data.switchEditor, //switchEditor
                -1, //index
                text.data.menu, //menu
                text.data.filename, //filename
                text.data.textHeight, //textHeight
                text.data.textWidth //textWidth
            }, //data end
            text.cursor.row >= text.textEdit.size() - 1 ? text.topLineNumber - 1 : text.topLineNumber, //topLineNumber
            text.topColumnNumber, //topColumnNumber
            text.highlightStart, //highlightStart
            text.highlightEnd //highlightEnd
        }; //Text end
    }

    return
    { //Text start
        text.textEdit.set(text.cursor.row,newLine), //textEdit
        {text.cursor.row, text.cursor.column - 1}, //cursor
        { //data start
            {false,false,false,false}, //menusToDraw
            text.data.isShift, //isShift
            text.data.isCaps, //isCaps
            text.data.isScroll, //isScroll
            text.data.isCtrl, //isCtrl
            text.data.clearHistory, //clearHistory
            text.data.switchEditor, //switchEditor
            -1, //index
            text.data.menu, //menu
            text.data.filename, //filename
            text.data.textHeight, //textHeight
            text.data.textWidth //textWidth
        }, //data end
        text.topLineNumber, //topLineNumber
        text.topColumnNumber, //topColumnNumber
        text.highlightStart, //highlightStart
        text.highlightEnd //hightlightEnd
    }; //Text end
}

better::Text better::newLine(better::Text textEdit, bool autoIndent) { //create cases for the following: newline at start of line, newline at end of line, newline in middle of line, newline at end of text
    int textHeight {textEdit.data.textHeight};
    int textWidth {textEdit.data.textWidth};

    std::vector<better::Text> texts {};

    bool endOfText {textEdit.cursor.row == textEdit.textEdit.size() - 1 ? true : false};
    bool textSizeRightSize {textEdit.textEdit.size() > (textHeight - 1) ? true : false}; //if end of text make sure to not just append a new row
    
    better::Text newText
    {
        endOfText ? textEdit.textEdit.set(textEdit.cursor.row, textEdit.textEdit[textEdit.cursor.row].take(textEdit.cursor.column)).push_back(textEdit.textEdit[textEdit.cursor.row].drop(textEdit.cursor.column)) :
                    textEdit.textEdit.insert(textEdit.cursor.row + 1, textEdit.textEdit[textEdit.cursor.row].drop(textEdit.cursor.column)).set(textEdit.cursor.row, textEdit.textEdit[textEdit.cursor.row].take(textEdit.cursor.column)), //textEdit
        {textEdit.cursor.row + 1, 0}, //cursor
        { //data start
            {false,false,false,false}, //menusToDraw
            textEdit.data.isShift, //isShift
            textEdit.data.isCaps, //isCaps
            textEdit.data.isScroll, //isScroll
            textEdit.data.isCtrl, //isCtrl
            textEdit.data.clearHistory, //clearHistory
            textEdit.data.switchEditor, //switchEditor
            -1, //index
            textEdit.data.menu, //menu
            textEdit.data.filename, //filename
            textEdit.data.textHeight, //textHeight
            textEdit.data.textWidth //textWidth
        }, //data end
        endOfText ? (textSizeRightSize ? textEdit.topLineNumber + 1 : 0) : textEdit.topLineNumber, //topLineNumber
        0, //index
        textEdit.highlightStart, //highlightStart
        textEdit.highlightEnd //highlightEnd
    };
    
    texts.push_back(newText);
    
    if(autoIndent) {
        int prevIndent {better::getPreviousIndentLevel(textEdit, textEdit.cursor.row)};
        for(int i{}; i < prevIndent; ++i) {
            texts.push_back(better::updateText(texts.back(),' '));
        }
    }
    
    return texts.back();
}

better::Cursor better::findCursorPos(int topLine, int topColumn, SDL_Event event, int columnOffset, int characterHeight, int characterWidth) { //change this to switch between different editors in splitscreen
    int relativeColumn {static_cast<int>(event.button.x / characterWidth)};
    int relativeRow {static_cast<int>(event.button.y / characterHeight) - 1};
    return better::Cursor {relativeRow + topLine, relativeColumn + topColumn - static_cast<int>(columnOffset / characterWidth)};
}

Uint8 better::getRed(Uint32 color) {
    return static_cast<Uint8>((color & 0xFF000000) >> 24);
}

Uint8 better::getGreen(Uint32 color) {
    return static_cast<Uint8>((color & 0x00FF0000) >> 16);
}

Uint8 better::getBlue(Uint32 color) {
    return static_cast<Uint8>((color & 0x0000FF00) >> 8);
}

Uint8 better::getAlpha(Uint32 color) {
    return static_cast<Uint8>(color & 0x000000FF);
}

better::Text better::scroll(better::Text text, SDL_Event event) {
    int row {text.topLineNumber + text.data.textHeight};
    int column {text.topColumnNumber + text.data.textWidth};

    if(event.wheel.y > 0) {
        return better::scrollUp(text, event.wheel.y, row);
    }
    else {
        return better::scrollDown(text, event.wheel.y, row);
    }

    if(event.wheel.x > 0) {
        return better::scrollRight(text, event.wheel.x, column, row);
    }
    else {
        return better::scrollLeft(text, event.wheel.x, column);
    }

    return text;
}

better::Text better::scrollLeft(better::Text text, int x, int column) {
    for(int times{}; times > x; --times) {
        if(text.topColumnNumber) {
            column -= 1;
            text.topColumnNumber -= 1;
        }
    }
    return text;
}

better::Text better::scrollRight(better::Text text, int x, int column, int row) {
    for(int times{}; times < x; ++times) {
        if((column == text.topColumnNumber + text.data.textWidth) && (column < text.textEdit[row].size())) {
            column += 1;
            text.topColumnNumber += 1;
        }
    }
    return text;
}

better::Text better::scrollDown(better::Text text, int y, int row) {
    for(int times{}; times > y; --times) {
        if((row == text.topLineNumber + text.data.textHeight) && (row - 1 < text.textEdit.size())) {
            row += 1;
            text.topLineNumber += 1;
        }
    }
    return text;
}

better::Text better::scrollUp(better::Text text, int y, int row) {
    for(int times{}; times < y; ++times) {
        if(text.topLineNumber) {
            row -= 1;
            text.topLineNumber -= 1;
        }
    }
    return text;
}

int better::getPreviousIndentLevel(better::Text text, int row) {
    int notSpace {0};
    for(int i{}; i < text.textEdit[row].size(); ++i) {
        if(text.textEdit[row][i] == ' ') {
            notSpace++;
        }
        else {
            break;
        }
    }
    return notSpace;    
}

better::Text better::tab(better::Text text) {
    return better::updateText(better::updateText(better::updateText(better::updateText(text,' '),' '),' '),' ');
}
