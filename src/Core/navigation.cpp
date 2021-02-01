#include "navigation.hpp"

better::Text better::horizontalNav(better::Text text, SDL_Keycode key) { //cases for if cursor is at top right/bottom left of screen
    int textWidth {text.data.textWidth};
    int textHeight {text.data.textHeight};
    better::Cursor cursor{text.cursor};
    switch(key) {
        case SDL_SCANCODE_RIGHT:
            if((cursor.row != text.textEdit.size() - 1) || (cursor.row == text.textEdit.size() - 1 && cursor.column < text.textEdit[cursor.row].size())) {
                if(cursor.column == text.textEdit[cursor.row].size()) {
                    cursor.column -= 1;
                    cursor.column = 0;
                    cursor.row += 1;
                    text.topColumnNumber = 0;
                }
                else {
                    cursor.column += 1;
                }
                if(cursor.column == text.topColumnNumber + textWidth) {
                    text.topColumnNumber += 1;
                }
            }
            break;

        case SDL_SCANCODE_LEFT:
            if((cursor.row) || (!cursor.row && cursor.column)) {
                if(!cursor.column) {
                    cursor.row -= 1;
                    cursor.column = text.textEdit[cursor.row].size();
                    if(cursor.column > textWidth - 2) {
                        text.topColumnNumber = (cursor.column - textWidth) + 1;
                    }
                }
                else {
                    cursor.column -= 1;
                }
                if((cursor.column == text.topColumnNumber) && (cursor.column)) {
                    text.topColumnNumber -= 1;
                }
            }
            break;
    }
    text.cursor = cursor;
    return text;
}

better::Text better::verticalNav(better::Text text, SDL_Keycode key) {
    switch(key) {
        case SDL_SCANCODE_DOWN:
            return better::downNav(text);
        case SDL_SCANCODE_UP:
            return better::upNav(text);
    }
    return text;
}

better::Text better::upNav(better::Text text) {
    if(text.cursor.row) {
        if((text.cursor.row == text.topLineNumber) && text.cursor.row) {
            text.topLineNumber -= 1;
        }
        if(text.textEdit[text.cursor.row - 1].size() < text.cursor.column) { //check the next row has less elements than current column of cursor position
            text.cursor.row -= 1;
            text.cursor.column = text.textEdit[text.cursor.row].size();
        }
        else {
            text.cursor.row -= 1;
        }
    }
    return text;
}

better::Text better::downNav(better::Text text) {
    if(text.cursor.row < text.textEdit.size()) {
        if((text.cursor.row == text.textEdit.size() - 1) || (text.cursor.row == text.topLineNumber + text.data.textHeight - 1 && text.topLineNumber + text.data.textHeight - 1 >= text.textEdit.size() - 1)) {
            return text;
        }
        if(text.textEdit[text.cursor.row + 1].size() < text.cursor.column) { //check the next row has less elements than current column of cursor position
            text.cursor.column -= 1;
            text.cursor.row += 1;
            text.cursor.column = text.textEdit[text.cursor.row].size();
        }
        else {
            text.cursor.row += 1;
        }
        if(text.cursor.row == text.topLineNumber + text.data.textHeight - 1 && text.topLineNumber + text.data.textHeight - 1 < text.textEdit.size()) {
            text.topLineNumber += 1; //scroll down
        }
    }
    return text;
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