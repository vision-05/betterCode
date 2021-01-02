//! @file
//! This file contains all of the user defined structs used throughout the project
//constexpr for textheight and textwidth, edit conditionals, get rid of ternaries
#include "datatypes.hpp"

better::Text better::updateText(better::Text textEdit, char newChar) {
    int textHeight {textEdit.data.textHeight};
    int textWidth {textEdit.data.textWidth};
    //change the topline number variable if needs to scroll
    bool endOfLine {textEdit.cursor.column == textEdit.textEdit[textEdit.cursor.row].size()};
    bool endOfScreen {textEdit.cursor.column + 1 >= textEdit.topColumnNumber + textWidth};
    immer::flex_vector<char> line {endOfLine ? textEdit.textEdit[textEdit.cursor.row].push_back(newChar) : textEdit.textEdit[textEdit.cursor.row].insert(textEdit.cursor.column, newChar)}; //add the character to the line (change to insert unless the cursor is at the end of the line)
    better::Text tempText {textEdit.textEdit.set(textEdit.cursor.row,line),{textEdit.cursor.row, textEdit.cursor.column + 1}, {{false,false,false,false},textEdit.data.isShift,textEdit.data.isCaps,textEdit.data.isScroll,textEdit.data.isCtrl,textEdit.data.clearHistory,textEdit.data.switchEditor,-1,textEdit.data.menu,textEdit.data.filename,textEdit.data.textHeight,textEdit.data.textWidth}, textEdit.topLineNumber, endOfScreen ? textEdit.topColumnNumber + 1 : textEdit.topColumnNumber, textEdit.highlightStart, textEdit.highlightEnd};
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
            return {text.textEdit.erase(text.cursor.row).set(text.cursor.row - 1, newLine), {text.cursor.row - 1, colNo}, {{false,false,false,false},text.data.isShift,text.data.isCaps,text.data.isScroll,text.data.isCtrl,text.data.clearHistory,text.data.switchEditor,-1,text.data.menu,text.data.filename,text.data.textHeight,text.data.textWidth}, text.cursor.row >= text.textEdit.size() - 1 ? text.topLineNumber - 1 : text.topLineNumber, topColumnNo, text.highlightStart, text.highlightEnd};
        }
        return {text.textEdit.erase(text.cursor.row), {text.cursor.row - 1, static_cast<int>(text.textEdit[text.cursor.row - 1].size())}, {{false,false,false,false},text.data.isShift,text.data.isCaps,text.data.isScroll,text.data.isCtrl,text.data.clearHistory,text.data.switchEditor,-1,text.data.menu,text.data.filename,text.data.textHeight,text.data.textWidth}, text.cursor.row >= text.textEdit.size() - 1 ? text.topLineNumber - 1 : text.topLineNumber, text.topColumnNumber, text.highlightStart, text.highlightEnd};
    }

    return {text.textEdit.set(text.cursor.row,newLine), {text.cursor.row, text.cursor.column - 1}, {{false,false,false,false},text.data.isShift,text.data.isCaps,text.data.isScroll,text.data.isCtrl,text.data.clearHistory,text.data.switchEditor,-1,text.data.menu,text.data.filename,text.data.textHeight,text.data.textWidth}, text.topLineNumber, text.topColumnNumber, text.highlightStart, text.highlightEnd};
}

better::Text better::newLine(better::Text textEdit) { //create cases for the following: newline at start of line, newline at end of line, newline in middle of line, newline at end of text
    int textHeight {textEdit.data.textHeight};
    int textWidth {textEdit.data.textWidth};

    std::vector<better::Text> texts {};

    bool endOfText {textEdit.cursor.row == textEdit.textEdit.size() - 1 ? true : false};
    bool textSizeRightSize {textEdit.textEdit.size() > (textHeight - 1) ? true : false}; //if end of text make sure to not just append a new row
    
    better::Text newText {endOfText ? textEdit.textEdit.set(textEdit.cursor.row, textEdit.textEdit[textEdit.cursor.row].take(textEdit.cursor.column)).push_back(textEdit.textEdit[textEdit.cursor.row].drop(textEdit.cursor.column)) : textEdit.textEdit.insert(textEdit.cursor.row + 1, textEdit.textEdit[textEdit.cursor.row].drop(textEdit.cursor.column)).set(textEdit.cursor.row, textEdit.textEdit[textEdit.cursor.row].take(textEdit.cursor.column)), {textEdit.cursor.row + 1, 0}, {{false,false,false,false},textEdit.data.isShift,textEdit.data.isCaps,textEdit.data.isScroll,textEdit.data.isCtrl,textEdit.data.clearHistory,textEdit.data.switchEditor,-1,textEdit.data.menu,textEdit.data.filename,textEdit.data.textHeight,textEdit.data.textWidth}, endOfText ? (textSizeRightSize ? textEdit.topLineNumber + 1 : 0) : textEdit.topLineNumber, 0, textEdit.highlightStart, textEdit.highlightEnd}; //insert newline unless at bottom
    int prevIndent {better::getPreviousIndentLevel(textEdit, textEdit.cursor.row)};
    texts.push_back(newText);
    for(int i{}; i < prevIndent; ++i) {
        texts.push_back(better::updateText(texts.back(),' '));
    }
    //create new case for between {},[] and ()
    return texts.back();
}

better::charMapArr better::makeCharMapArr(::Uint8 charArray[16]) {
    better::charMapArr tempArr {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    for(int i{}; i < 16; ++i) {
        tempArr.arr[i] = charArray[i];
    }
    return tempArr;
}

better::Cursor better::findCursorPos(int topLine, int topColumn, SDL_Event event, int columnOffset) { //change this to switch between different editors in splitscreen
    int relativeColumn {static_cast<int>(event.button.x / 8)};
    int relativeRow {static_cast<int>(event.button.y / 16) - 1};
    return better::Cursor {relativeRow + topLine, relativeColumn + topColumn - static_cast<int>(columnOffset / 8)};
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
