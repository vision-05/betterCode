//! @file
//! This is the main file of the project, containing the main event loop and renderer
//fix splitscreen bugs: menu clicking
#define SDL_MAIN_HANDLED //reformat code and fix highlighting bugs!!! Also create loading bar for save
//add scroll bars for horizontal and vertical nav
#include <SDL2/SDL.h>
#include <immer-0.6.2/immer/flex_vector.hpp>
#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>

#include "datatypes.hpp"
#include "fileUtils.hpp"
#include "renderchars.hpp"
#include "menubar.hpp"

//Progress bar for saving, etc (maybe try save sound)

namespace better { //TODO: highlighting (create a separate buffer of colors, create async function to parse text and highlight)

//! better::verticalNav scrolls the text vertically based on arrow keys. This is done incrementing or decrementing the variable topLine.
//! The topLine variable tells the render function to start rendering at that line in the text buffer.

better::Text verticalNav(better::Text text, SDL_Keycode key);

//! better::horizontalNav essentially scrolls the text horizontally based on arrow keys. This is done incrementing or decrementing the variable topColumn.
//! The topColumn variable tells the render function to start rendering at that position in each line of the text buffer.

better::Text horizontalNav(better::Text text, SDL_Keycode key);

//! better::shift "shifts" every key on the keyboard that has an alternate key.
//! This function is currently configured for ascii Us Windows keyboards in ISO configuration.

char shift(char key);

//! better::shiftLetter "shifts" characters for when caps lock is pressed.
//! Non letters are unaffected as caps lock only capitalises letters.

char shiftLetter(char key);

//! better::unshiftLetter "unshifts" characters while both the shift key and caps lock are pressed.
//! This function only unshifts letters.

char unshiftLetter(char key);

//! better::edit1 initialises a text editor the size of the window passed by its pointer.
//! It also reads in the file into its own text buffer, and contains the event loop for that text edit.

better::Text mouseMotion(better::Text text, SDL_Event event, SDL_Surface* surface, SDL_Cursor* guiCursor, int columnOffset, int editorCount);

better::Text keyDown(better::Text text, SDL_Event event, SDL_Surface* surface, SDL_Cursor* guiCursor, int columnOffset, int editorCount);

better::Text mouseButton(better::Text text, SDL_Event event, SDL_Surface* surface, SDL_Cursor* guiCursor, int columnOffset, int editorCount);

better::Text mouseButtonUp(better::Text text, SDL_Event event, SDL_Surface* surface, SDL_Cursor* guiCursor, int columnOffset, int editorCount);

better::Text mouseWheel(better::Text text, SDL_Event event, SDL_Surface* surface, SDL_Cursor* guiCursor, int columnOffset, int editorCount);

better::Text autoBracket(better::Text text, char bracket);

better::Text pasteClipboard(better::Text text);

better::Text deleteHighlighted(better::Text text);

better::Text handleKey(better::Text text, char key);

void edit1(SDL_Window* window, std::string filename, int textHeight, int textWidth, Uint32 colorbg, Uint32 colorfg, Uint32 colorhighlight, Uint32 colorparens, Uint32 colorcomments);

void copyClipboard(better::Text text);

better::Text cutClipboard(better::Text text);

better::Text ctrlShortcuts(better::Text text);

void resetMenus(bool* menus);

int selectMenu(bool menus[]);

void quitApp(SDL_Cursor* cursor, SDL_Surface* surface, SDL_Window* window);

bool operator!=(better::Text lhs, better::Text rhs);

}

int main(int argc, char* argv[]) {
    SDL_SetMainReady();
    SDL_Init(SDL_INIT_VIDEO);

    std::string filename {better::fileDialog().string()};

    SDL_Window* window = SDL_CreateWindow("Better Code",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          1200,960,SDL_WINDOW_RESIZABLE);

    int textHeight {60};
    int textWidth {150};
    Uint32 colorbg {0x222222FF};
    Uint32 colorfg {0x5588AAFF};
    Uint32 colorhighlight {0x444444FF};
    Uint32 colorparens {0xAA6969FF};
    Uint32 colorcomments {0x666666FF};
    
    better::edit1(window, filename, textHeight, textWidth, colorbg, colorfg, colorhighlight, colorparens, colorcomments);
    return 0;
}

bool better::operator!=(better::Text lhs, better::Text rhs) {
    return lhs.textEdit != rhs.textEdit;
}

better::Text better::pasteClipboard(better::Text text) {
    std::string clipboardText {};
    std::vector<better::Text> tempTexts {text};
    if(SDL_HasClipboardText()) {
        clipboardText = SDL_GetClipboardText();
        clipboardText.pop_back();
        clipboardText.pop_back();
        for(int i{}; i < clipboardText.size(); ++i) {
            if(clipboardText[i] == '\n') {
                tempTexts.push_back(better::newLine(tempTexts.back()));
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
    for(int i{text.highlightStart.row}; i <= text.highlightEnd.row; ++i) {
        for(int j{}; j < text.textEdit[i].size(); ++j) {
            if(text.highlightStart.row == text.highlightEnd.row && j < text.highlightEnd.column && j >= text.highlightStart.column) {
                clipboardText.push_back(text.textEdit[i][j]);
            }
            else if(text.highlightStart.row == text.highlightEnd.row) {
                continue;
            }
            else if(i == text.highlightStart.row && j >= text.highlightStart.column) {
                clipboardText.push_back(text.textEdit[i][j]);
            }
            else if(i == text.highlightEnd.row && j < text.highlightEnd.column) {
                clipboardText.push_back(text.textEdit[i][j]);
            }
            else if(i > text.highlightStart.row && i < text.highlightEnd.row) {
                clipboardText.push_back(text.textEdit[i][j]);
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

void better::edit1(SDL_Window* window, std::string filename, int textHeight, int textWidth, Uint32 colorbg, Uint32 colorfg, Uint32 colorhighlight, Uint32 colorparens, Uint32 colorcomments) {
    std::vector<std::vector<better::Text>> texts {{}};
    int editorWidth {1200};
    int editorHeight {960};
    int editorIndex {0};
    int editorCount {1};
    int columnOffset {editorIndex * (editorWidth / editorCount)};

    SDL_Cursor* guiCursor {SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM)};
    SDL_SetCursor(guiCursor);

    SDL_Rect screen {.x = 0, .y = 16, .w = 1200, .h = 960 - 16};

    SDL_Surface* surface = SDL_GetWindowSurface(window);

    std::string menus {" File  Edit  View  Settings "};
    std::vector<std::vector<std::string>> menuText {{"Open","Save","Exit"},{"Cut","Copy","Paste"},{},{"Edit Settings"}};
    better::Text firstText {better::readFile(filename), {0,0}, {{false,false,false,false}, false, false, false, false, false, 0, -1, {}, filename, 60, 150}, 0, 0, {0,0}, {0,0}};
    texts[editorIndex].push_back(firstText);
    
    SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 0x22, 0x22, 0x22, 0xFF));
    better::renderText(surface, texts[editorIndex].back().textEdit, texts[editorIndex].back().topLineNumber, texts[editorIndex].back().topColumnNumber, textHeight, textWidth, {0,0}, {0,0}, colorbg, colorfg, colorhighlight, colorparens, colorcomments, columnOffset);
    SDL_Rect topline {.x = 0, .y = 0, .w = 1200, .h = 16};
    SDL_FillRect(surface, &topline, SDL_MapRGBA(surface->format, 0x66, 0x66, 0x66, 0xFF));
    better::drawMenuBar(surface, menus, 0xDDDDDDFF, 0x666666FF, editorWidth, columnOffset);

    SDL_UpdateWindowSurface(window);

    SDL_Event event;

    std::unordered_map<Uint32, better::Text(*)(better::Text, SDL_Event, SDL_Surface*, SDL_Cursor*, int, int)> handlers {
        {SDL_MOUSEMOTION, better::mouseMotion},
        {SDL_KEYDOWN, better::keyDown},
        {SDL_MOUSEBUTTONDOWN, better::mouseButton},
        {SDL_MOUSEWHEEL, better::mouseWheel},
        {SDL_MOUSEBUTTONUP, better::mouseButtonUp}
    };

    while(1) {
        if(SDL_WaitEvent(&event)) {
            if(texts[editorIndex].back().cursor.row == -1) {
                texts[editorIndex].back().data.isScroll = true;
            }
            const Uint8* state {SDL_GetKeyboardState(NULL)};
            if(state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT]) {
                texts[editorIndex].back().data.isShift = true;
            }
            if(!state[SDL_SCANCODE_LSHIFT] && !state[SDL_SCANCODE_RSHIFT]) {
                texts[editorIndex].back().data.isShift = false;
            }
            if(state[SDL_SCANCODE_RCTRL] || state[SDL_SCANCODE_LCTRL]) {
                texts[editorIndex].back().data.isCtrl = true;
            }
            if(!state[SDL_SCANCODE_RCTRL] && !state[SDL_SCANCODE_LCTRL]) {
                texts[editorIndex].back().data.isCtrl = false;
            }

            if(event.type == SDL_QUIT) {
                better::quitApp(guiCursor, surface, window);
                return;
            }
            else if(event.type == SDL_WINDOWEVENT) {
                if(event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    screen.h = event.window.data2 - 16;
                    screen.w = event.window.data1;
                    for(int i{}; i < texts.size(); ++i) {
                        for(int j{}; j < texts[i].size(); ++j) {
                            texts[i][j].data.textHeight = event.window.data2 / 16;
                            texts[i][j].data.textWidth = (event.window.data1 / 8) / texts.size();
                            editorWidth = event.window.data1;
                            editorHeight = event.window.data2;
                        }
                    }
                    SDL_FreeSurface(surface);
                    surface = SDL_GetWindowSurface(window);
                    SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 0x22, 0x22, 0x22, 0xFF));
                    for(int i{}; i < texts.size(); ++i) {
                        columnOffset = i * (editorWidth / editorCount);
                        topline.w = editorWidth;
                        SDL_FillRect(surface, &topline, SDL_MapRGBA(surface->format, 0x66, 0x66, 0x66, 0xFF));
                        better::drawMenuBar(surface, menus, 0xDDDDDDFF, 0x666666FF, event.window.data1, columnOffset);
                        better::renderText(surface, texts[i].back().textEdit, texts[i].back().topLineNumber, texts[i].back().topColumnNumber, texts[i].back().data.textHeight, texts[i].back().data.textWidth, texts[i].back().highlightStart, texts[i].back().highlightEnd, colorbg, colorfg, colorhighlight, colorparens, colorcomments, columnOffset);
                    }
                    SDL_UpdateWindowSurface(window);
                    continue;
                }
            }

            if(event.type == SDL_KEYDOWN && texts[editorIndex].back().data.isCtrl) {
                if(event.key.keysym.sym == 'z') {
                    if(texts.size() > 1) {
                        texts[editorIndex].pop_back();
                    }
                }
                else if(event.key.keysym.sym == 'n') {
                    if(texts.size() < 3) {
                        editorIndex++;
                        editorCount++;
                        filename = better::fileDialog().string();
                        texts.push_back({{better::readFile(filename), {0,0}, {{false,false,false,false}, false, false, false, false, false, editorIndex, -1, {}, filename, static_cast<int>(editorHeight / 16), static_cast<int>((editorWidth / (texts.size() + 1)) / 8)}, 0, 0, {0,0}, {0,0}}});
                        for(int i{}; i < texts.size(); ++i) {
                            for(int j{}; j < texts[i].size(); ++j) {
                                texts[i][j].data.textWidth = static_cast<int>((editorWidth / editorCount) / 8);
                            }
                        }
                    }
                }
                else if(event.key.keysym.sym == 's') {
                    better::saveFile(texts[editorIndex].back().textEdit, texts[editorIndex].back().data.filename);
                }
                else if(event.key.keysym.sym == 'o') {
                    better::saveFile(texts[editorIndex].back().textEdit, texts[editorIndex].back().data.filename);
                    std::string tempFilename {};
                    tempFilename = better::fileDialog().string();
                    texts[editorIndex].back().data.clearHistory = true;
                    texts[editorIndex].back().textEdit = better::readFile(tempFilename);
                    texts[editorIndex].back().cursor = {0,0};
                    texts[editorIndex].back().topLineNumber = 0;
                    texts[editorIndex].back().topColumnNumber = 0;
                    texts[editorIndex].back().highlightStart = {0,0};
                    texts[editorIndex].back().highlightEnd = {0,0};
                    texts[editorIndex].back().data.index = -1;
                    texts[editorIndex].back().data.isScroll = false;
                    texts[editorIndex].back().data.isCaps = false;
                    texts[editorIndex].back().data.isShift = false;
                    texts[editorIndex].back().data.isCtrl = false;
                    texts[editorIndex].back().data.menu.clear();
                    texts[editorIndex].back().data.filename = tempFilename;
                }
                else if(event.key.keysym.sym == 'c') {
                    better::copyClipboard(texts[editorIndex].back());
                }
                else if(event.key.keysym.sym == 'v') {
                    texts[editorIndex].push_back(better::pasteClipboard(texts[editorIndex].back()));
                }
                else if(event.key.keysym.sym == 'x') {
                    texts[editorIndex].push_back(better::cutClipboard(texts[editorIndex].back()));
                }
            }

            else if(event.type == SDL_KEYDOWN || event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEWHEEL || event.type == SDL_MOUSEBUTTONUP) {
                columnOffset = editorIndex * (editorWidth / editorCount);
                better::Text tempText {(handlers[event.type])(texts[editorIndex].back(), event, surface, guiCursor, columnOffset, editorCount)};
                if(tempText.data.clearHistory) {
                    texts[editorIndex].clear();
                    texts[editorIndex].push_back(tempText);
                    texts[editorIndex].back().data.clearHistory = false;
                }
                else if(tempText.data.switchEditor != texts[editorIndex].back().data.switchEditor) {
                    editorIndex = tempText.data.switchEditor;
                    columnOffset = editorIndex * static_cast<int>(editorWidth / editorCount);
                }
                else if(tempText != texts[editorIndex].back()) {
                    texts[editorIndex].push_back(tempText);
                }
                else {
                    texts[editorIndex].pop_back();
                    texts[editorIndex].push_back(tempText);
                }
            }
            else {
                continue;
            }

            SDL_FillRect(surface, &screen, SDL_MapRGBA(surface->format, 0x22, 0x22, 0x22, 0xFF));
            for(int i{}; i < texts.size(); ++i) {
                columnOffset = i * static_cast<int>(editorWidth / editorCount);
                better::drawMenuBar(surface, menus, 0xDDDDDDFF, 0x666666FF, editorWidth, columnOffset);
                better::renderText(surface, texts[i].back().textEdit, texts[i].back().topLineNumber, texts[i].back().topColumnNumber, texts[i].back().data.textHeight, texts[i].back().data.textWidth, texts[i].back().highlightStart, texts[i].back().highlightEnd, colorbg, colorfg, colorhighlight, colorparens, colorcomments, columnOffset);
                if(!texts[i].back().data.isScroll) {
                    better::renderCursor(surface, texts[i].back().cursor.column, texts[i].back().cursor.row, texts[i].back().topLineNumber, texts[i].back().topColumnNumber, columnOffset);
                }
                texts[i].back().data.index = better::selectMenu(texts[i].back().data.menusToDraw);
                if(texts[i].back().data.index < 0 || texts[i].back().data.index > 3) {
                    texts[i].back().data.menu.clear();
                }
                else {
                    texts[i].back().data.menu = menuText[texts[i].back().data.index];
                }

                better::drawMenus(surface, texts[i].back().data.menu, 0xDDDDDDFF, colorhighlight, columnOffset);
            }
            SDL_UpdateWindowSurface(window);
        }
    }
}

better::Text better::mouseButtonUp(better::Text text, SDL_Event event, SDL_Surface* surface, SDL_Cursor* guiCursor, int columnOffset, int editorCount) {
    better::Cursor tempCursor = better::findCursorPos(text.topLineNumber, text.topColumnNumber, event, columnOffset);
    if(tempCursor.row == -1) {
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
    return text;
}

better::Text better::autoBracket(better::Text text, char letter) {
    int num {letter == '(' ? 1 : 2};
    return better::horizontalNav(better::updateText(better::updateText(text,letter),letter + num),SDL_SCANCODE_LEFT);
}

better::Text better::mouseWheel(better::Text text, SDL_Event event, SDL_Surface* surface, SDL_Cursor* guiCursor, int columnOffset, int editorCount) {
    text.data.isScroll = true;
    return better::scroll(text, event);
}

better::Text better::keyDown(better::Text text, SDL_Event event, SDL_Surface* surface, SDL_Cursor* guiCursor, int columnOffset, int editorCount) {
    if(event.key.keysym.scancode == SDL_SCANCODE_DOWN || event.key.keysym.scancode == SDL_SCANCODE_UP) {
        text.highlightStart = text.highlightEnd;
        return better::verticalNav(text, event.key.keysym.scancode);
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
            texts.push_back(better::newLine(better::newLine(texts.back())));
            texts.push_back(better::verticalNav(texts.back(),SDL_SCANCODE_UP));
            texts.push_back(better::tab(texts.back()));
        }
        else {
            if(texts.back().cursor.row != texts.back().textEdit.size() - 1 && texts.back().cursor.row >= texts.back().topLineNumber + texts.back().data.textHeight - 2) {
                texts.back().topLineNumber += 1;
            }
            texts.push_back(better::newLine(texts.back()));
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
better::Text better::mouseButton(better::Text text, SDL_Event event, SDL_Surface* surface, SDL_Cursor* guiCursor, int columnOffset, int editorCount) {
    better::Cursor tempCursor {better::findCursorPos(text.topLineNumber, text.topColumnNumber, event, columnOffset)};
    if(editorCount > 1) {
        if(editorCount == 2) {
            if(tempCursor.column < 0) {
                text.data.switchEditor = 0;
                return text;
            }
            else if(!columnOffset) {
                if(tempCursor.column > text.data.textWidth) {
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
                else if(tempCursor.column > text.data.textWidth) {
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
            else if(!columnOffset) {
                if(tempCursor.column > text.data.textWidth && tempCursor.column < 2 * text.data.textWidth) {
                    text.data.switchEditor = 1;
                    return text;
                }
                else if(tempCursor.column > 2 * text.data.textWidth) {
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
    if((event.button.y / 16) - 1 == -1) {
        tempCursor.column = static_cast<int>((event.button.x - columnOffset) / 8);
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

    else if((better::selectMenu(text.data.menusToDraw) > -1) && ((((event.button.x - columnOffset) / 8) > 20) || ((event.button.y / 16) > text.data.menu.size()))){
        better::resetMenus(text.data.menusToDraw);
    }

    int selectedMenu {better::selectMenu(text.data.menusToDraw)};
    int clickRow {event.button.y / 16};
    int clickColumn {event.button.x / 8};
    if(selectedMenu == 0) {
        if(clickRow == 2) {
            better::saveFile(text.textEdit, text.data.filename);
            better::resetMenus(text.data.menusToDraw);
            return text;
        }
        else if(clickRow == 1) {
            better::saveFile(text.textEdit, text.data.filename);
            std::string tempFilename = better::fileDialog().string();
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
better::Text better::mouseMotion(better::Text text, SDL_Event event, SDL_Surface* surface, SDL_Cursor* guiCursor, int columnOffset, int editorCount) {
    if((event.motion.y / 16) == 0) {
        SDL_FreeCursor(guiCursor);
        guiCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    }
    else {
        guiCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
    }
    SDL_SetCursor(guiCursor);
    return text;
}

void better::quitApp(SDL_Cursor* cursor, SDL_Surface* surface, SDL_Window* window) {
    SDL_FreeCursor(cursor);
    SDL_FreeSurface(surface);
    SDL_DestroyWindow(window);
    SDL_Quit();
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

better::Text better::verticalNav(better::Text text, SDL_Keycode key) {
    int textHeight {text.data.textHeight};
    int textWidth {text.data.textWidth};
    better::Cursor cursor {text.cursor};
    switch(key) {
        case SDL_SCANCODE_DOWN:
            if(cursor.row < text.textEdit.size()) {
                if((cursor.row == text.textEdit.size() - 1) || (cursor.row == text.topLineNumber + textHeight - 1 && text.topLineNumber + textHeight - 1 >= text.textEdit.size() - 1)) {
                    return text;
                }
                if(text.textEdit[cursor.row + 1].size() < cursor.column) { //check the next row has less elements than current column of cursor position
                    cursor.column -= 1;
                    cursor.column = text.textEdit[cursor.row + 1].size();
                    cursor.row += 1;
                }
                else {
                    cursor.row += 1;
                }
                if(cursor.row == text.topLineNumber + textHeight - 1 && text.topLineNumber + textHeight - 1 < text.textEdit.size()) {
                    text.topLineNumber += 1; //scroll down
                }
            }
            break;

        case SDL_SCANCODE_UP:
            if(text.cursor.row) {
                if((cursor.row == text.topLineNumber) && cursor.row) {
                    text.topLineNumber -= 1;
                }
                if(text.textEdit[text.cursor.row - 1].size() < cursor.column) { //check the next row has less elements than current column of cursor position
                    cursor.column = text.textEdit[cursor.row - 1].size();
                    cursor.row -= 1;
                }
                else {
                    cursor.row -= 1;
                }
            }
            break;
    }
    text.cursor = cursor;
    return text;
}

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
