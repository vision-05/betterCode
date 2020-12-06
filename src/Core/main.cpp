#define SDL_MAIN_HANDLED

#include <SDL2-2.0.12/include/SDL.h>
#include <immer/flex_vector.hpp>
#include <vector>
#include <thread>

#include "datatypes.hpp"
#include "fileUtils.hpp"
#include "renderchars.hpp"

//Splitscreen for 2 screens
//Add menu
//Add tab system
//Progress bar for saving, etc
//2 screens should be separately editable

namespace better { //TODO: highlighting text, shortcuts

better::Text verticalNav(better::Text text, SDL_Keycode key);
better::Text horizontalNav(better::Text text, SDL_Keycode key);

char shift(char key);
char shiftLetter(char key);
char unshiftLetter(char key);
void edit1(SDL_Window* window, std::string filename);

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

    SDL_Surface* surface = SDL_GetWindowSurface(window);
    
    better::Text firstText {better::readFile(filename), {0,0}, 0, 0};
    texts.push_back(firstText);

    SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 0x22, 0x22, 0x22, 0xFF));
    better::renderText(surface, texts.back().textEdit, texts.back().topLineNumber, texts.back().topColumnNumber);
    better::renderCursor(surface, texts.back().cursor.column, texts.back().cursor.row, texts.back().topLineNumber, texts.back().topColumnNumber);

    SDL_UpdateWindowSurface(window);

    bool isScroll {false};
    bool isShift {false};
    bool capsLock {false};

    SDL_Event event;

    while(1) {
        if(SDL_WaitEvent(&event)) {
            const Uint8* state {SDL_GetKeyboardState(NULL)};
            if(state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT]) { //get the shifts working properly!!!
                isShift = true;
            }
            if(!state[SDL_SCANCODE_LSHIFT] && !state[SDL_SCANCODE_RSHIFT]) {
                isShift = false;
            }

            if(event.type == SDL_QUIT) {
                SDL_FreeSurface(surface);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return;
            }

            else if(event.type == SDL_KEYDOWN && (event.key.keysym.scancode == SDL_SCANCODE_DOWN || event.key.keysym.scancode == SDL_SCANCODE_UP)) {
                texts[texts.size() - 1] = better::verticalNav(texts.back(), event.key.keysym.scancode);
            }

            else if(event.type == SDL_KEYDOWN && (event.key.keysym.scancode == SDL_SCANCODE_RIGHT || event.key.keysym.scancode == SDL_SCANCODE_LEFT)) {
                texts[texts.size() - 1] = better::horizontalNav(texts.back(), event.key.keysym.scancode);
            }

            else if(event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
                texts.push_back(better::newLine(texts.back()));
            }

            else if(event.type == SDL_MOUSEWHEEL) { //set bool scroll to true
                texts[texts.size() - 1] = better::scroll(texts.back(), event, 60, 150);
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
                texts[texts.size() - 1].cursor = tempCursor;
                isScroll = false;
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

                switch(key) {
                    case '\b':
                        texts.push_back(better::backspace(texts.back()));
                        break;
                    default:
                        texts.push_back(better::updateText(texts.back(),key)); //save the text at its current state (find out why newline weird behaviour/still printing newline)
                }
            }
            SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 0x22, 0x22, 0x22, 0xFF));
            better::renderText(surface, texts.back().textEdit, texts.back().topLineNumber, texts.back().topColumnNumber);
            if(!isScroll) {
                better::renderCursor(surface, texts.back().cursor.column, texts.back().cursor.row, texts.back().topLineNumber, texts.back().topColumnNumber);
            }
            SDL_UpdateWindowSurface(window);
        }
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
    else if(key == '9') {
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
    else if(key == '\'') {
        key += 25;
    }
    return key;
}

better::Text better::verticalNav(better::Text text, SDL_Keycode key) {
    const int textHeight {60};
    const int textWidth {150};
    switch(key) {
        case SDL_SCANCODE_DOWN:
            if(text.cursor.row < text.textEdit.size()) {
                if((text.cursor.row == text.textEdit.size() - 1) || (text.cursor.row == text.topLineNumber + textHeight && text.topLineNumber + textHeight >= text.textEdit.size() - 1)) {
                    return text;
                }
                if(text.textEdit[text.cursor.row + 1].size() < text.cursor.column) { //check the next row has less elements than current column of cursor position
                    text.cursor.column = text.textEdit[text.cursor.row + 1].size();
                    text.cursor.row += 1;
                }
                else {
                    text.cursor.row += 1;
                }
                if(text.cursor.row == text.topLineNumber + textHeight && text.topLineNumber + textHeight < text.textEdit.size()) {
                    text.topLineNumber += 1; //scroll down
                }
            }
            break;

        case SDL_SCANCODE_UP:
            if(text.cursor.row) {
                if(text.textEdit[text.cursor.row - 1].size() < text.cursor.column) { //check the next row has less elements than current column of cursor position
                    text.cursor.column = text.textEdit[text.cursor.row - 1].size();
                    text.cursor.row -= 1;
                }
                else {
                    text.cursor.row -= 1;
                }
                if((text.cursor.row == text.topLineNumber) && text.cursor.row) {
                    text.topLineNumber -= 1;
                }
            }
            break;
    }
    return text;
}

better::Text better::horizontalNav(better::Text text, SDL_Keycode key) {
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