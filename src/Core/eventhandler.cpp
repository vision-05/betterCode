#include "eventhandler.hpp"

better::Text better::pasteClipboard(better::Text text) {
    std::string clipboardText {};
    std::vector<better::Text> tempTexts {text};
    if(SDL_HasClipboardText()) {
        clipboardText = SDL_GetClipboardText();
        clipboardText.pop_back();
        for(int i{}; i < clipboardText.size(); ++i) {
            if(clipboardText[i] == '\n') {
                tempTexts.push_back(better::newLine(tempTexts.back(), false));
            }
            else if(clipboardText[i] == '\r') {
                continue;
            }
            else {
                tempTexts.push_back(better::updateText(tempTexts.back(), clipboardText[i]));
            }
        }
        return tempTexts.back();
    }
    return text;
}

void better::copyClipboard(better::Text text) { //fix copying if dragging backwards to highlight
    std::string clipboardText {};
    for(int row{text.highlightStart.row}; row <= text.highlightEnd.row; ++row) {
        for(int column{}; column < text.textEdit[row].size(); ++column) {
            if(text.highlightStart.row == text.highlightEnd.row && column <= text.highlightEnd.column && column >= text.highlightStart.column) {
                clipboardText.push_back(text.textEdit[row][column]);
            }
            else if(text.highlightStart.row == text.highlightEnd.row) {
                continue;
            }
            else if(row == text.highlightStart.row && column >= text.highlightStart.column) {
                clipboardText.push_back(text.textEdit[row][column]);
            }
            else if(row == text.highlightEnd.row && column <= text.highlightEnd.column) {
                clipboardText.push_back(text.textEdit[row][column]);
            }
            else if(row > text.highlightStart.row && row < text.highlightEnd.row) {
                clipboardText.push_back(text.textEdit[row][column]);
            }
        }
        clipboardText.push_back('\n');
    }
    SDL_SetClipboardText(clipboardText.c_str());
}

better::Text better::deleteHighlighted(better::Text text) {
    std::vector<better::Text> tempTexts {text};
    tempTexts.back().cursor = text.highlightEnd;
    tempTexts.back().cursor.column += 1;
    int stopColumn {0};
    for(int i{tempTexts.back().cursor.row}; i >= tempTexts.back().highlightStart.row; --i) {
        if(i == tempTexts.back().highlightStart.row) {
            stopColumn = tempTexts.back().highlightStart.column + 1;
        }
        for(int j{tempTexts.back().cursor.column}; j >= stopColumn; --j) {
            tempTexts.push_back(better::backspace(tempTexts.back()));
        }
    }
    tempTexts.back().highlightEnd = tempTexts.back().highlightStart;
    return tempTexts.back();
}

better::Text better::cutClipboard(better::Text text) {
    better::copyClipboard(text);
    return better::deleteHighlighted(text);
}

better::Text better::autoBracket(better::Text text, char letter) {
    int num {letter == '(' ? 1 : 2};
    return better::horizontalNav(better::updateText(better::updateText(text,letter),letter + num),SDL_SCANCODE_LEFT);
}

better::Text better::mouseButtonUp(better::Text text, SDL_Event event, SDL_Surface* surface, SDL_Cursor* guiCursor, int columnOffset, int editorCount, ConfigData config) {
    better::Cursor tempCursor = better::findCursorPos(text.topLineNumber, text.topColumnNumber, event, columnOffset, config.characterHeight, config.characterWidth);
    if(event.button.button == SDL_BUTTON_LEFT) {
        if(event.button.y < config.characterHeight) {
            return text;
        }
        text.highlightEnd = tempCursor;
        if(text.highlightEnd.row < text.highlightStart.row || (text.highlightEnd.row == text.highlightStart.row && text.highlightEnd.column < text.highlightStart.column)) {
            better::Cursor tempCursor = text.highlightEnd;
            text.highlightEnd = text.highlightStart;
            text.highlightStart = tempCursor;
            if(text.textEdit[text.cursor.row].size() > text.highlightEnd.column) {
                text.cursor.column += 1;
            }
            else {
                text.cursor.row += 1;
                text.cursor.column = 0;
            }
        }
    }
    return text;
}

better::Text better::mouseWheel(better::Text text, SDL_Event event, SDL_Surface* surface, SDL_Cursor* guiCursor, int columnOffset, int editorCount, ConfigData config) {
    text.data.isScroll = true;
    return better::scroll(text, event);
}

better::Text better::keyDown(better::Text text, SDL_Event event, SDL_Surface* surface, SDL_Cursor* guiCursor, int columnOffset, int editorCount, ConfigData config) {
    if(event.key.keysym.scancode == SDL_SCANCODE_DOWN || event.key.keysym.scancode == SDL_SCANCODE_UP) {
        text.highlightStart = text.highlightEnd;
        return better::verticalNav(text, event.key.keysym.scancode);
    }
    
    else if(event.key.keysym.scancode == SDL_SCANCODE_DELETE) {
        better::Text tempText {better::horizontalNav(text,SDL_SCANCODE_RIGHT)};
        return better::backspace(tempText);
    }

    else if(event.key.keysym.scancode == SDL_SCANCODE_RIGHT || event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
        text.highlightStart = text.highlightEnd;
        return better::horizontalNav(text, event.key.keysym.scancode);
    }
    else if(event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
        text.highlightStart = text.highlightEnd;
        std::vector<better::Text> texts {text}; //fix scrolling here for newline near bottom of text
        
        if((text.cursor.column - 1 > -1) && ((text.textEdit[text.cursor.row][text.cursor.column - 1] == '{' && text.textEdit[text.cursor.row][text.cursor.column] == '}') || (text.textEdit[text.cursor.row][text.cursor.column - 1] == '(' && text.textEdit[text.cursor.row][text.cursor.column] == ')'))) {
            if(texts.back().cursor.row != texts.back().textEdit.size() - 1 && texts.back().cursor.row >= texts.back().topLineNumber + texts.back().data.textHeight - 2) {
                texts.back().topLineNumber += 3;
            }
            texts.push_back(better::newLine(better::newLine(texts.back(), true), true));
            texts.push_back(better::verticalNav(texts.back(),SDL_SCANCODE_UP));
            texts.push_back(better::tab(texts.back()));
        }
        else {
            if(texts.back().cursor.row != texts.back().textEdit.size() - 1 && texts.back().cursor.row >= texts.back().topLineNumber + texts.back().data.textHeight - 2) {
                texts.back().topLineNumber += 1;
            }
            texts.push_back(better::newLine(texts.back(), true));
        }
        return texts.back();
    }

    else if(event.key.keysym.scancode == SDL_SCANCODE_TAB) {
        return better::tab(text);
    }

    else {
        SDL_Keycode keycode {event.key.keysym.scancode};
        if(keycode == SDL_SCANCODE_CAPSLOCK) {
            text.data.isCaps ? text.data.isCaps = false : text.data.isCaps = true;
            return text;
        }
        else if(keycode == SDL_SCANCODE_LSHIFT || keycode == SDL_SCANCODE_RSHIFT) {
            return text;
        }
        else if(keycode == SDL_SCANCODE_RCTRL || keycode == SDL_SCANCODE_LCTRL) {
            return text;
        }
        char key = {static_cast<char>(event.key.keysym.sym)};
        if(text.data.isShift) {
            key = better::shift(key);
            if(text.data.isCaps) {
                key = better::unshiftLetter(key);
            }
            text.data.isShift = false;
        }
        if(text.data.isCaps) {
            key = better::shiftLetter(key);
        }
        if(text.data.isScroll) { //change this
            text.topLineNumber = text.cursor.row;
            text.topColumnNumber = 0;
        }
        if(text.data.isCtrl) {
            text.highlightEnd = text.highlightStart;
            return text;
        }
        if(text.cursor.row == -1) {
            text.cursor.row = 0;
            better::resetMenus(text.data.menusToDraw);
        }
        if((text.highlightStart.row != text.highlightEnd.row) || (text.highlightEnd.column > text.highlightStart.column)) {
            if(key == '\b') {
                text.highlightStart.column += 1;
            }
            return better::handleKey(better::deleteHighlighted(text),key);
        }
    text.highlightEnd = text.cursor;
    text.highlightStart = text.cursor;
    return better::handleKey(text, key);
        
    }
    return text;
}

better::Text better::handleKey(better::Text text, char key) {
    text.highlightStart = text.cursor;
    text.highlightEnd = text.cursor;
    switch(key) {
        case '\b':
            return better::backspace(text);
        case '\'':
            return better::horizontalNav(better::updateText(better::updateText(text,'\''),'\''),SDL_SCANCODE_LEFT);
        case '\"':
            return better::horizontalNav(better::updateText(better::updateText(text,key),'\"'),SDL_SCANCODE_LEFT);
        default:
            if(key == '(' || key == '{' || key == '[') {
                return better::autoBracket(text,key);
            }
            else {
                return better::updateText(text,key);
            }
    }
}

//cases separate for left and right click, break up this function
better::Text better::mouseButton(better::Text text, SDL_Event event, SDL_Surface* surface, SDL_Cursor* guiCursor, int columnOffset, int editorCount, ConfigData config) {
    int lineNoOffset {static_cast<int>(std::to_string(text.textEdit.size()).size()) + 1};
    better::Cursor tempCursor {better::findCursorPos(text.topLineNumber, text.topColumnNumber, event, columnOffset, config.characterHeight, config.characterWidth)};
    if(editorCount > 1) {
        if(editorCount == 2) {
            if(tempCursor.column < 0) {
                text.data.switchEditor = 0;
                return text;
            }
            else if(text.data.switchEditor == 0) {
                if(tempCursor.column > (lineNoOffset + text.data.textWidth)) {
                    text.data.switchEditor = 1;
                    return text;
                }
            }
        }
        else {
            if(text.data.switchEditor == 1) {
                if(tempCursor.column < 0) {
                    text.data.switchEditor = 0;
                    return text;
                }
                else if(tempCursor.column > (text.data.textWidth + lineNoOffset)) {
                    text.data.switchEditor = 2;
                    return text;
                }
            }
            else if(text.data.switchEditor == 2) {
                if(tempCursor.column < 0) {
                    if(tempCursor.column + text.data.textWidth < -1) {
                        text.data.switchEditor = 0;
                        return text;
                    }
                    else if(tempCursor.column + text.data.textWidth > -1) {
                        text.data.switchEditor = 1;
                        return text;
                    }
                }
            }
            else if(text.data.switchEditor == 0) {
                if(tempCursor.column > text.data.textWidth && tempCursor.column < 2 * (lineNoOffset + text.data.textWidth)) {
                    text.data.switchEditor = 1;
                    return text;
                }
                else if(tempCursor.column > 2 * (text.data.textWidth + lineNoOffset)) {
                    text.data.switchEditor = 2;
                    return text;
                }
            }
        }
    }
    if(tempCursor.row > text.textEdit.size() - 1) {
        tempCursor.row = text.textEdit.size() - 1;
        tempCursor.column = text.textEdit[text.textEdit.size() - 1].size();
    }
    if(tempCursor.column > text.textEdit[tempCursor.row].size()) {
        tempCursor.column = text.textEdit[tempCursor.row].size();
    }
    if((event.button.y / config.characterHeight) - 1 == -1) {
        tempCursor.column = static_cast<int>((event.button.x - columnOffset) / config.characterWidth);
        tempCursor.row = -1;
        text.data.isScroll = true;
        better::resetMenus(text.data.menusToDraw);
        if(tempCursor.column < 6) {
            text.data.menusToDraw[0] = true;
        }
        else if(tempCursor.column > 5 && tempCursor.column < 12) {
            text.data.menusToDraw[1] = true;
        }
        else if(tempCursor.column > 11 && tempCursor.column < 18) {
            text.data.menusToDraw[2] = true;
        }
        else if(tempCursor.column > 17 && tempCursor.column < 28) {
            text.data.menusToDraw[3] = true;
        }
        tempCursor.column = 0;
    }

    else if((better::selectMenu(text.data.menusToDraw) > -1) && ((((event.button.x - columnOffset) / config.characterWidth) > 20) || ((event.button.y / 16) > text.data.menu.size()))){
        better::resetMenus(text.data.menusToDraw);
    }

    int selectedMenu {better::selectMenu(text.data.menusToDraw)};
    int clickRow {event.button.y / config.characterHeight};
    int clickColumn {event.button.x / config.characterWidth};
    if(selectedMenu == 0) {
        if(clickRow == 2) {
            better::saveFile(text.textEdit, text.data.filename);
            better::resetMenus(text.data.menusToDraw);
            return text;
        }
        else if(clickRow == 1) {
            better::saveFile(text.textEdit, text.data.filename);
            std::filesystem::path workspaceFolder {text.data.filename};
            workspaceFolder = workspaceFolder.parent_path();
            std::string tempFilename = better::fileDialog(config, workspaceFolder).string();
            text.data.clearHistory = true;
            text.textEdit = better::readFile(tempFilename);
            text.cursor = {0,0};
            text.topLineNumber = 0;
            text.topColumnNumber = 0;
            text.highlightStart = {0,0};
            text.highlightEnd = {0,0};
            text.data.index = -1;
            text.data.isScroll = false;
            text.data.isCaps = false;
            text.data.isShift = false;
            text.data.isCtrl = false;
            text.data.menu.clear();
            text.data.filename = tempFilename;
            better::resetMenus(text.data.menusToDraw);
            return text;
        }
        else if(clickRow == 3) {
            better::resetMenus(text.data.menusToDraw);
            return text;
        }
    }
    else if(selectedMenu == 1) {
        if(clickRow == 1) {
            better::resetMenus(text.data.menusToDraw);
            return better::cutClipboard(text);
        }
        else if(clickRow == 2) {
            better::copyClipboard(text);
            better::resetMenus(text.data.menusToDraw);
            return text;
        }
        else if(clickRow == 3) {
            better::resetMenus(text.data.menusToDraw);
            return better::pasteClipboard(text);
        }
    }
    
    if(tempCursor.row != -1 ) {
        text.data.isScroll = false;
        text.cursor = tempCursor;
        text.highlightStart = tempCursor;
        text.highlightEnd = tempCursor;
    }
    return text;
}

//cases for mouse over context menu or dropdown menu
better::Text better::mouseMotion(better::Text text, SDL_Event event, SDL_Surface* surface, SDL_Cursor* guiCursor, int columnOffset, int editorCount, ConfigData config) {
    if((event.motion.y / config.characterHeight) == 0) {
        SDL_FreeCursor(guiCursor);
        guiCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    }
    else {
        guiCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
    }
    SDL_SetCursor(guiCursor);
    return text;
}

char better::shiftLetter(char key) {
    if(key >= 'a' && key <= 'z') {
        key -= 32;
    }
    return key;
}

char better::unshiftLetter(char key) {
    if(key >= 'A' && key <= 'Z') {
        key += 32;
    }
    return key;
}

char better::shift(char key) {
    std::unordered_map<char, char> shifted = {
        {32,32},{33,33},{34,34},{35,126},{36,36},{37,37},{38,38},{39,64},
        {40,40},{41,41},{42,42},{43,43},{44,60},{45,95},{46,62},{47,63},
        {48,41},{49,33},{50,34},{51,35},{52,36},{53,37},{54,94},{55,38},
        {56,42},{57,40},{58,58},{59,58},{60,60},{61,43},{62,62},{63,63},
        {64,64},{65,65},{66,66},{67,67},{68,68},{69,69},{70,70},{71,71},
        {72,72},{73,73},{74,74},{75,75},{76,76},{77,77},{78,78},{79,79},
        {80,80},{81,81},{82,82},{83,83},{84,84},{85,85},{86,86},{87,87},
        {88,88},{89,89},{90,90},{91,123},{92,124},{93,125},{94,94},{95,95},
        {96,96},{97,65},{98,66},{99,67},{100,68},{101,69},{102,70},{103,71},
        {104,72},{105,73},{106,74},{107,75},{108,76},{109,77},{110,78},{111,79},
        {112,80},{113,81},{114,82},{115,83},{116,84},{117,85},{118,86},{119,87},
        {120,88},{121,89},{122,90},{123,123},{124,124},{125,125},{126,126}
    };
    return shifted[key];
}

int better::selectMenu(bool menus[]) {
    for(int i{}; i < 4; ++i) {
        if(menus[i]) {
            return i;
        }
    }
    return -1;
}

void better::resetMenus(bool* menus) {
    for(int i{}; i < 4; ++i) {
        menus[i] = false;
    }
}