//! @file
//! This is the main file of the project, containing the main event loop and renderer

#define SDL_MAIN_HANDLED
//i added the comment to test saving
#include <SDL2-2.0.12/include/SDL.h>
#include <immer/flex_vector.hpp>
#include <vector>
#include <thread>
#include <string>
#include <iostream>
#include "datatypes.hpp"
#include "fileUtils.hpp"
#include "renderchars.hpp"
#include "menubar.hpp"

//Splitscreen for 2 screens
//Add menu
//Add tab system
//Progress bar for saving, etc
//2 screens should be separately editable

namespace better { //TODO: highlighting text, shortcuts

//! better::verticalNav scrolls the text vertically based on arrow keys. This is done incrementing or decrementing the variable topLine.
//! The topLine variable tells teh render function to start rendering at that line in the text buffer.

better::Text verticalNav(better::Text text, SDL_Keycode key, const int textHeight, const int textWidth);

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

void edit1(SDL_Window* window, std::string filename);

void resetMenus(bool* menus);

int selectMenu(bool menus[]);

}

int main(int argc, char* argv[]) {
    SDL_SetMainReady();
    SDL_Init(SDL_INIT_VIDEO);

    std::string filename {better::fileDialog().string()};

    SDL_Window* window = SDL_CreateWindow("Better Code",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          1200,960,0);
    
    better::edit1(window, filename);
    return 0;
}

void better::edit1(SDL_Window* window, std::string filename) {
    std::vector<better::Text> texts {};

    SDL_Cursor* guiCursor {SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM)};
    SDL_SetCursor(guiCursor);

    SDL_Rect screen {.x = 0, .y = 16, .w = 1200, .h = 960 - 16};

    SDL_Surface* surface = SDL_GetWindowSurface(window);

    std::string menus {" File  Edit  View  Settings "};
    std::vector<std::vector<std::string>> menuText {{"Open","Save","Exit"},{"Cut","Copy","Paste"},{},{"Edit Settings"}};
    
    better::Text firstText {better::readFile(filename), {0,0}, 0, 0};
    texts.push_back(firstText);

    SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 0x22, 0x22, 0x22, 0xFF));
    better::renderText(surface, texts.back().textEdit, texts.back().topLineNumber, texts.back().topColumnNumber, 60, 150);
    better::renderCursor(surface, texts.back().cursor.column, texts.back().cursor.row, texts.back().topLineNumber, texts.back().topColumnNumber);
    better::drawMenuBar(surface, menus, 0xDDDDDDFF, 0x666666FF, 1200);

    SDL_UpdateWindowSurface(window);

    bool isScroll {false};
    bool isShift {false};
    bool capsLock {false};

    bool menusToDraw[4] {false,false,false,false};
    std::vector<std::string> menu {};
    int index {};

    SDL_Event event;

    while(1) {
        if(SDL_WaitEvent(&event)) {
            if(texts.back().cursor.row == -1) {
                isScroll = true;
            }
            const Uint8* state {SDL_GetKeyboardState(NULL)};
            if(state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT]) {
                isShift = true;
            }
            if(!state[SDL_SCANCODE_LSHIFT] && !state[SDL_SCANCODE_RSHIFT]) {
                isShift = false;
            }

            if(event.type == SDL_QUIT) {
                SDL_FreeCursor(guiCursor);
                SDL_FreeSurface(surface);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return;
            }

            else if(event.type == SDL_MOUSEMOTION) {
                if((event.motion.y / 16) == 0) {
                    SDL_FreeCursor(guiCursor);
                    guiCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
                    SDL_SetCursor(guiCursor);
                }
                else {
                    guiCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
                    SDL_SetCursor(guiCursor);
                }
            }

            else if(event.type == SDL_KEYDOWN && (event.key.keysym.scancode == SDL_SCANCODE_DOWN || event.key.keysym.scancode == SDL_SCANCODE_UP)) {
                texts.back() = better::verticalNav(texts.back(), event.key.keysym.scancode, 60, 150);
            }

            else if(event.type == SDL_KEYDOWN && (event.key.keysym.scancode == SDL_SCANCODE_RIGHT || event.key.keysym.scancode == SDL_SCANCODE_LEFT)) {
                texts.back() = better::horizontalNav(texts.back(), event.key.keysym.scancode);
            }

            else if(event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
                texts.push_back(better::newLine(texts.back()));
            }

            else if(event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_TAB) {
                better::Text tempText {texts.back()};
                for(int i{}; i < 4; ++i) {
                    tempText = better::updateText(tempText, ' ');
                }
                texts.push_back(tempText);
            }

            else if(event.type == SDL_MOUSEWHEEL) { //set bool scroll to true
                texts.back() = better::scroll(texts.back(), event, 60, 150);
                isScroll = true;
            }

            else if(event.type == SDL_MOUSEBUTTONDOWN) {
                better::Cursor tempCursor {better::findCursorPos(texts.back().topLineNumber, texts.back().topColumnNumber, event)};
                if(tempCursor.row > texts.back().textEdit.size() - 1) {
                    tempCursor.row = texts.back().textEdit.size() - 1;
                    tempCursor.column = 0;
                }
                if(tempCursor.column > texts.back().textEdit[tempCursor.row].size()) {
                    tempCursor.column = texts.back().textEdit[tempCursor.row].size();
                }

                if((event.button.y / 16) - 1 == -1) {
                    tempCursor.column = event.button.x / 8;
                    tempCursor.row = -1;
                    isScroll = true;
                    better::resetMenus(menusToDraw);
                    if(tempCursor.column < 6) {
                        menusToDraw[0] = true;
                    }
                    else if(tempCursor.column > 5 && tempCursor.column < 12) {
                        menusToDraw[1] = true;
                    }
                    else if(tempCursor.column > 11 && tempCursor.column < 18) {
                        menusToDraw[2] = true;
                    }
                    else if(tempCursor.column > 17 && tempCursor.column < 28) {
                        menusToDraw[3] = true;
                    }
                    tempCursor.column = 0;
                    continue;
                }

                else if((better::selectMenu(menusToDraw) > -1) && (((event.button.x / 8) > 20) || ((event.button.y / 16) > menu.size()))){
                    better::resetMenus(menusToDraw);
                }

                int selectedMenu {better::selectMenu(menusToDraw)};
                int clickRow {event.button.y / 16};
                int clickColumn {event.button.x / 8};
                if(selectedMenu > -1) {
                    if(clickColumn < 21) {
                        if(clickRow < menu.size() && event.button.y > 16) {
                            if(selectedMenu == 0) {
                                if(clickRow == 2) {
                                    better::saveFile(texts.back().textEdit, filename);
                                    better::resetMenus(menusToDraw);
                                }
                                else if(clickRow == 1) {
                                    better::saveFile(texts.back().textEdit, filename);
                                    filename = better::fileDialog().string();
                                    texts.clear();
                                    texts.push_back({better::readFile(filename), {0,0}, 0, 0});
                                    better::resetMenus(menusToDraw);
                                }
                                else if(clickRow == 3) {
                                    SDL_FreeCursor(guiCursor);
                                    SDL_FreeSurface(surface);
                                    SDL_DestroyWindow(window);
                                    SDL_Quit();
                                    return;
                                }
                            }
                            else if(selectedMenu == 1) {
                                if(clickRow == 1) {
                                    
                                }
                                else if(clickRow == 2) {
                                    
                                }
                                else if(clickRow == 3) {
                                    
                                }
                            }
                        }
                    }
                }
                texts.back().cursor = tempCursor;
                if(tempCursor.row != -1) {
                    isScroll = false;
                }
            }

            else if(event.type == SDL_KEYDOWN) {
                SDL_Keycode keycode {event.key.keysym.scancode};
                if(keycode == SDL_SCANCODE_CAPSLOCK) {
                    capsLock ? capsLock = false : capsLock = true;
                    continue;
                }
                if(keycode == SDL_SCANCODE_LSHIFT || keycode == SDL_SCANCODE_RSHIFT) {
                    continue;
                }
                char key = {static_cast<char>(event.key.keysym.sym)};
                if(isShift) {
                    key = better::shift(key);
                    if(capsLock) {
                        key = better::unshiftLetter(key);
                    }
                    isShift = false;
                }
                if(capsLock) {
                    key = better::shiftLetter(key);
                }
                if(isScroll) {
                    texts[texts.size() - 1].topLineNumber = texts.back().cursor.row;
                    texts[texts.size() - 1].topColumnNumber = 0;
                }
                if(texts.back().cursor.row == -1) {
                    texts.back().cursor.row = 0;
                    better::resetMenus(menusToDraw);
                }

                switch(key) {
                    case '\b':
                        texts.push_back(better::backspace(texts.back()));
                        break;
                    case '(':
                        texts.push_back(better::updateText(better::updateText(texts.back(),key),')'));
                        texts.back() = better::horizontalNav(texts.back(),SDL_SCANCODE_LEFT);
                        break;
                    case '{':
                        texts.push_back(better::updateText(better::newLine(better::newLine(better::updateText(texts.back(),key))),'}'));
                        texts.back() = better::horizontalNav(texts.back(),SDL_SCANCODE_LEFT);
                        texts.back() = better::verticalNav(texts.back(), SDL_SCANCODE_UP, 60, 150);
                        break;
                    case '[':
                        texts.push_back(better::updateText(better::updateText(texts.back(),key),']'));
                        texts.back() = better::horizontalNav(texts.back(),SDL_SCANCODE_LEFT);
                        break;
                    case '<':
                        texts.push_back(better::updateText(better::updateText(texts.back(),key),'>'));
                        texts.back() = better::horizontalNav(texts.back(),SDL_SCANCODE_LEFT);
                        break;
                    case '\'':
                        texts.push_back(better::updateText(better::updateText(texts.back(),'\''),'\''));
                        texts.back() = better::horizontalNav(texts.back(),SDL_SCANCODE_LEFT);
                        break;
                    case '\"':
                        texts.push_back(better::updateText(better::updateText(texts.back(),key),'\"'));
                        texts.back() = better::horizontalNav(texts.back(),SDL_SCANCODE_LEFT);
                        break;
                    default:
                        texts.push_back(better::updateText(texts.back(),key));
                }
            }
            SDL_FillRect(surface, &screen, SDL_MapRGBA(surface->format, 0x22, 0x22, 0x22, 0xFF));
            better::renderText(surface, texts.back().textEdit, texts.back().topLineNumber, texts.back().topColumnNumber, 60, 150);
            if(!isScroll) {
                better::renderCursor(surface, texts.back().cursor.column, texts.back().cursor.row, texts.back().topLineNumber, texts.back().topColumnNumber);
            }
            index = better::selectMenu(menusToDraw);
            if(index < 0 || index > 3) {
                menu.clear();
            }
            else {
                menu = menuText[index];
            }

            better::drawMenus(surface, menu, 0xDDDDDDFF, 0x444444FF);

            SDL_UpdateWindowSurface(window);
        }
    }
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
    if(key >= 'a' && key <= 'z') {
        key -= 32;
    }
    else if(key >= '1' && key <= '5') {
        key -= 16;
    }
    else if(key == '8') {
        key -= 14;
    }
    else if(key == '6') {
        key += 40;
    }
    else if(key == '9' || key == '7') {
        key -= 17;
    }
    else if(key == '0') {
        key -= 7;
    }
    else if(key == '-') {
        key += 50;
    }
    else if(key == '=') {
        key -= 18;
    }
    else if(key == '.' || key == ',' || key == '/') {
        key  += 16;
    }
    else if(key == '[' || key == ']' || key == '\\') {
        key += 32;
    }
    else if(key == ';') {
        key -= 1;
    }
    else if(key == '#') {
        key += 91;
    }
    else if(key == 39) {
        key += 25;
    }
    return key;
}

better::Text better::verticalNav(better::Text text, SDL_Keycode key, const int textHeight, const int textWidth) {
    switch(key) {
        case SDL_SCANCODE_DOWN:
            if(text.cursor.row < text.textEdit.size()) {
                if((text.cursor.row == text.textEdit.size() - 1) || (text.cursor.row == text.topLineNumber + textHeight - 1 && text.topLineNumber + textHeight - 1 >= text.textEdit.size() - 1)) {
                    return text;
                }
                if(text.textEdit[text.cursor.row + 1].size() < text.cursor.column) { //check the next row has less elements than current column of cursor position
                    text.cursor.column = text.textEdit[text.cursor.row + 1].size();
                    text.cursor.row += 1;
                }
                else {
                    text.cursor.row += 1;
                }
                if(text.cursor.row == text.topLineNumber + textHeight - 1 && text.topLineNumber + textHeight - 1 < text.textEdit.size()) {
                    text.topLineNumber += 1; //scroll down
                }
            }
            break;

        case SDL_SCANCODE_UP:
            if(text.cursor.row) {
                if((text.cursor.row == text.topLineNumber) && text.cursor.row) {
                    text.topLineNumber -= 1;
                }
                if(text.textEdit[text.cursor.row - 1].size() < text.cursor.column) { //check the next row has less elements than current column of cursor position
                    text.cursor.column = text.textEdit[text.cursor.row - 1].size();
                    text.cursor.row -= 1;
                }
                else {
                    text.cursor.row -= 1;
                }
            }
            break;
    }
    return text;
}

better::Text better::horizontalNav(better::Text text, SDL_Keycode key) { //cases for if cursor is at top right/bottom left of screen
    const int textHeight {60};
    const int textWidth {150};
    switch(key) {
        case SDL_SCANCODE_RIGHT:
            if((text.cursor.row != text.textEdit.size() - 1) || (text.cursor.row == text.textEdit.size() - 1 && text.cursor.column < text.textEdit[text.cursor.row].size())) {
                if(text.cursor.column == text.textEdit[text.cursor.row].size()) {
                    text.cursor.column = 0;
                    text.cursor.row += 1;
                    text.topColumnNumber = 0;
                }
                else {
                    text.cursor.column += 1;
                }
                if(text.cursor.column == text.topColumnNumber + textWidth) {
                    text.topColumnNumber += 1;
                }
            }
            break;

        case SDL_SCANCODE_LEFT:
            if((text.cursor.row) || (!text.cursor.row && text.cursor.column)) {
                if(!text.cursor.column) {
                    text.cursor.row -= 1;
                    text.cursor.column = text.textEdit[text.cursor.row].size();
                    if(text.cursor.column > textWidth - 2) {
                        text.topColumnNumber = (text.cursor.column - textWidth) + 1;
                    }
                }
                else {
                    text.cursor.column -= 1;
                }
                if((text.cursor.column == text.topColumnNumber) && (text.cursor.column)) {
                    text.topColumnNumber -= 1;
                }
            }
            break;
    }
    return text;
}
