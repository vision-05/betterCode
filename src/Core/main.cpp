//! @file
//! This is the main file of the project, containing the main event loop and renderer
//fix splitscreen bugs: menu clicking
#define SDL_MAIN_HANDLED //reformat code and fix highlighting bugs!!! Also create loading bar for save
//add scroll bars for horizontal and vertical nav
//move navigation functions to new file
#include <SDL2/SDL.h>
#include <immer-0.6.2/immer/flex_vector.hpp>
#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <memory>

#include "datatypes.hpp"
#include "fileUtils.hpp"
#include "renderchars.hpp"
#include "menubar.hpp"
#include "navigation.hpp"
#include "eventhandler.hpp"

//Progress bar for saving, etc (maybe try save sound)

namespace better { //TODO: highlighting (create a separate buffer of colors, create async function to parse text and highlight)

void eventLoop(SDL_Window* window, std::string filename, int textHeight, int textWidth, better::ConfigData config);

void quitApp(SDL_Cursor* cursor, SDL_Surface* surface, SDL_Window* window);

bool operator!=(better::Text lhs, better::Text rhs);

better::Text openFile(std::string filename, int editorCount, int editorHeight, int editorWidth, int editorIndex, better::ConfigData config);

}

int main(int argc, char* argv[]) {
    SDL_SetMainReady();
    SDL_Init(SDL_INIT_VIDEO);

    better::ConfigData config
    {
        .foregroundColor = 0x5588AAFF,
        .backgroundColor = 0x222222FF,
        .identifierColor = 0x5588AAFF,
        .keywordColor = 0x5588AAFF,
        .symbolColor = 0xAA6969FF,
        .cursorColor = 0xAA7070FF,
        .lineNoColor = 0x5588AAFF,
        .highlightColor = 0x444444FF,
        .commentColor = 0x666666FF,
        .lineSelectColor = 0x333333FF,
        .menuColor = 0x666666FF,
        .menuBarColor = 0x666666FF,
        .menuTextColor = 0xDDDDDDFF,
        .characterHeight = 16,
        .characterWidth = 8
    };

    std::string filename {};

    if(argc == 2) {
        filename = (std::filesystem::current_path() / std::string(argv[1])).string();
    }
    else {
        filename = better::fileDialog(config).string();
    }

    SDL_Window* window = SDL_CreateWindow("Better Code",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          1200,960,SDL_WINDOW_RESIZABLE);

    int textHeight {60};
    int textWidth {149};
    
    better::eventLoop(window, filename, textHeight, textWidth, config);
    return 0;
}

bool better::operator!=(better::Text lhs, better::Text rhs) {
    return lhs.textEdit != rhs.textEdit;
}



better::Text better::openFile(std::string filename, int editorCount, int editorHeight, int editorWidth, int editorIndex, ConfigData config) {
    immer::flex_vector<immer::flex_vector<char>> tempText {better::readFile(filename)};
    return
    {
        tempText,
        {0,0},
        {
            {false,false,false,false},
            false,
            false,
            false,
            false,
            false,
            editorIndex,
            -1,
            std::vector<std::string>(),
            filename,
            (editorHeight / config.characterHeight) - 1,
            (((editorWidth / editorCount) / config.characterWidth) - 1) - (static_cast<int>(std::to_string(tempText.size()).size()) + 1)
        },
        0,
        0,
        {0,0},
        {0,0}
    };
}

void better::eventLoop(SDL_Window* window, std::string filename, int textHeight, int textWidth, ConfigData config) {
    std::vector<std::vector<better::Text>> texts {{}};
    int editorWidth {config.characterWidth * 150};
    int editorHeight {config.characterHeight * 60};
    int editorIndex {0};
    int editorCount {1};
    int columnOffset {editorIndex * (editorWidth / editorCount)};
    int lineNoOffset {0};

    SDL_Cursor* guiCursor {SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM)};
    SDL_SetCursor(guiCursor);

    SDL_Rect screen {.x = 0, .y = config.characterHeight, .w = editorWidth, .h = editorHeight - config.characterHeight};

    SDL_Surface* surface = SDL_GetWindowSurface(window);

    std::string menus {" File  Edit  View  Settings + X "};
    std::vector<std::vector<std::string>> menuText {{"Open","Save","Exit"},{"Cut","Copy","Paste"},{},{"Edit Settings"}};
    better::Text firstText {better::readFile(filename), {0,0}, {{false,false,false,false}, false, false, false, false, false, 0, -1, {}, filename, 60, 149}, 0, 0, {0,0}, {0,0}};
    firstText.data.textWidth -= (std::to_string(firstText.textEdit.size()).size() + 1);
    texts[editorIndex].push_back(firstText);
    lineNoOffset = config.characterWidth * (static_cast<int>(std::to_string(firstText.textEdit.size()).size()) + 1);
    
    SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, better::getRed(config.backgroundColor), better::getGreen(config.backgroundColor), better::getBlue(config.backgroundColor), better::getAlpha(config.backgroundColor)));
    better::renderText(surface, texts[0].back(), config,  columnOffset + lineNoOffset, texts[0].back());
    SDL_Rect topline {.x = 0, .y = 0, .w = config.characterWidth * 150, .h = config.characterHeight};
    SDL_FillRect(surface, &topline, SDL_MapRGBA(surface->format, better::getRed(config.menuBarColor), better::getGreen(config.menuBarColor), better::getBlue(config.menuBarColor), better::getAlpha(config.menuBarColor)));
    better::drawMenuBar(surface, menus, 0xDDDDDDFF, 0x666666FF, editorWidth, columnOffset, config.characterHeight, config.characterWidth);
    better::renderLineNumbers(surface, config, texts[0].back().data, texts[0].back().topLineNumber, columnOffset, texts[0].back().textEdit.size(), editorHeight);

    SDL_UpdateWindowSurface(window);

    SDL_Event event;

    std::unordered_map<Uint32, better::Text(*)(better::Text, SDL_Event, SDL_Surface*, SDL_Cursor*, int, int, ConfigData)> handlers {
        {SDL_KEYDOWN, better::keyDown},
        {SDL_MOUSEBUTTONDOWN, better::mouseButton},
        {SDL_MOUSEWHEEL, better::mouseWheel},
        {SDL_MOUSEBUTTONUP, better::mouseButtonUp}
    };

    bool firstTime {false};

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
                    screen.h = event.window.data2 - config.characterHeight;
                    screen.w = event.window.data1;
                    for(int i{}; i < texts.size(); ++i) {
                        for(int j{}; j < texts[i].size(); ++j) {
                            texts[i][j].data.textHeight = event.window.data2 / config.characterHeight;
                            texts[i][j].data.textWidth = (event.window.data1 / config.characterWidth) / texts.size() - 1;
                            texts[i][j].data.textWidth -= (std::to_string(texts[i][j].textEdit.size()).size() + 1);
                            editorWidth = event.window.data1;
                            editorHeight = event.window.data2;
                        }
                    }
                    SDL_FreeSurface(surface);
                    surface = SDL_GetWindowSurface(window);
                    SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, better::getRed(config.backgroundColor), better::getGreen(config.backgroundColor), better::getBlue(config.backgroundColor), better::getAlpha(config.backgroundColor)));
                    for(int i{}; i < texts.size(); ++i) {
                        lineNoOffset = (config.characterWidth * (std::to_string(texts[i].back().textEdit.size()).size() + 1));
                        columnOffset = (i * (editorWidth / editorCount));
                        topline.w = editorWidth;
                        SDL_FillRect(surface, &topline, SDL_MapRGBA(surface->format, better::getRed(config.menuBarColor), better::getGreen(config.menuBarColor), better::getBlue(config.menuBarColor), better::getAlpha(config.menuBarColor)));
                        better::drawMenuBar(surface, menus, 0xDDDDDDFF, 0x666666FF, event.window.data1, columnOffset, config.characterHeight, config.characterWidth);
                        better::renderText(surface, texts[i].back(), config, columnOffset + lineNoOffset, texts[i].back());
                        better::renderLineNumbers(surface, config, texts[i].back().data, texts[i].back().topLineNumber, texts[i].back().topColumnNumber, texts[i].back().textEdit.size(), editorHeight);
                    }
                    SDL_UpdateWindowSurface(window);
                    continue;
                }
            }

            if(event.type == SDL_KEYDOWN && texts[editorIndex].back().data.isCtrl) {
                if(event.key.keysym.sym == 'z') {
                    if(texts[editorIndex].size() > 1) { //change to search for last time editor had different character buffer
                        texts[editorIndex].pop_back();
                    }
                }
                else if(event.key.keysym.sym == 'n') {
                    if(texts.size() < 3) {
                        editorIndex++;
                        editorCount++;
                        std::filesystem::path workspaceFolder {texts[editorIndex - 1].back().data.filename};
                        workspaceFolder = workspaceFolder.parent_path();
                        filename = better::fileDialog(config, workspaceFolder).string();
                        texts.push_back({better::openFile(filename, editorCount, editorHeight, editorWidth, editorIndex, config)});
                        for(int i{}; i < texts.size(); ++i) {
                            for(int j{}; j < texts[i].size(); ++j) {
                                texts[i][j].data.textWidth = (((editorWidth / editorCount) / config.characterWidth) - 1) - (std::to_string(texts[editorIndex].back().textEdit.size()).size() + 1);
                            }
                        }
                        SDL_FillRect(surface, &screen, SDL_MapRGB(surface->format, better::getRed(config.backgroundColor), better::getGreen(config.backgroundColor), better::getBlue(config.backgroundColor)));
                        firstTime = true;
                    }
                }
                else if(event.key.keysym.sym == 's') {
                    better::saveFile(texts[editorIndex].back().textEdit, texts[editorIndex].back().data.filename);
                }
                else if(event.key.keysym.sym == 'o') {
                    better::saveFile(texts[editorIndex].back().textEdit, texts[editorIndex].back().data.filename);
                    std::string tempFilename {};
                    std::filesystem::path workspaceFolder {texts[editorIndex].back().data.filename};
                    workspaceFolder = workspaceFolder.parent_path();
                    tempFilename = better::fileDialog(config, workspaceFolder).string();
                    texts[editorIndex].push_back(better::openFile(filename, editorCount, editorHeight, editorWidth, editorIndex, config));
                    texts[editorIndex].back().data.clearHistory = true;
                    SDL_FillRect(surface, &screen, SDL_MapRGB(surface->format, better::getRed(config.backgroundColor), better::getGreen(config.backgroundColor), better::getBlue(config.backgroundColor)));
                    firstTime = true;
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
                else if(event.key.keysym.scancode == SDL_SCANCODE_LCTRL || event.key.keysym.scancode == SDL_SCANCODE_RCTRL) {
                    continue;
                }
                texts[editorIndex].back().highlightStart = texts[editorIndex].back().cursor;
                texts[editorIndex].back().highlightEnd = texts[editorIndex].back().cursor;
            }

            else if(event.type == SDL_MOUSEMOTION) {
                better::mouseMotion(texts[editorIndex].back(), event, surface, guiCursor, columnOffset + lineNoOffset, editorCount, config);
                continue;
            }

            else if(event.type == SDL_KEYDOWN || event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEWHEEL || event.type == SDL_MOUSEBUTTONUP) {
                columnOffset = editorIndex * (editorWidth / editorCount);
                better::Text tempText {(handlers[event.type])(texts[editorIndex].back(), event, surface, guiCursor, columnOffset + lineNoOffset, editorCount, config)};
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
                    texts[editorIndex].push_back(tempText);
                }
            }
            else {
                continue;
            }
            lineNoOffset = (8 * (std::to_string(texts[editorIndex].back().textEdit.size()).size() + 1));
            texts[editorIndex].back().data.textWidth = (((editorWidth / editorCount) / config.characterWidth) - 1) - (std::to_string(texts[editorIndex].back().textEdit.size()).size() + 1);

            //SDL_FillRect(surface, &screen, SDL_MapRGBA(surface->format, better::getRed(config.backgroundColor), better::getGreen(config.backgroundColor), better::getBlue(config.backgroundColor), better::getAlpha(config.backgroundColor)));
            for(int i{}; i < texts.size(); ++i) {
                lineNoOffset = (config.characterWidth * (std::to_string(texts[i].back().textEdit.size()).size() + 1));
                columnOffset = (i * static_cast<int>(editorWidth / editorCount));
                better::drawMenuBar(surface, menus, 0xDDDDDDFF, 0x666666FF, editorWidth, columnOffset, config.characterHeight, config.characterWidth);
                bool notNew {texts[i].size() > 1};
                better::renderText(surface, texts[i].back(), config, columnOffset + lineNoOffset, notNew ? (firstTime ? texts[i].back() : texts[i][texts[i].size() - 2]) : texts[i].back(), notNew ? false : (firstTime ? true : false));
                
                if(!texts[i].back().data.isScroll) {
                    better::renderCursor(surface, texts[i].back().cursor, config, texts[i].back().topLineNumber, texts[i].back().topColumnNumber, columnOffset + lineNoOffset);
                }
                texts[i].back().data.index = better::selectMenu(texts[i].back().data.menusToDraw);
                if(texts[i].back().data.index < 0 || texts[i].back().data.index > 3) {
                    texts[i].back().data.menu.clear();
                }
                else {
                    texts[i].back().data.menu = menuText[texts[i].back().data.index];
                }

                better::renderLineNumbers(surface, config, texts[i].back().data, texts[i].back().topLineNumber, columnOffset, texts[i].back().textEdit.size(), editorHeight);
                better::drawMenus(surface, texts[i].back().data.menu, 0xDDDDDDFF, config.highlightColor, columnOffset, config.characterHeight, config.characterWidth);
            }
            SDL_UpdateWindowSurface(window);
            firstTime = false;
        }
    }
}



void better::quitApp(SDL_Cursor* cursor, SDL_Surface* surface, SDL_Window* window) {
    SDL_FreeCursor(cursor);
    SDL_FreeSurface(surface);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
