#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

#include <SDL2/SDL.h>
#include <immer-0.6.2/immer/flex_vector.hpp>

#include "datatypes.hpp"
#include "fileUtils.hpp"
#include "navigation.hpp"

namespace better {

better::Text mouseMotion(better::Text text, SDL_Event event, SDL_Surface* surface, SDL_Cursor* guiCursor, int columnOffset, int editorCount, better::ConfigData config);

better::Text keyDown(better::Text text, SDL_Event event, SDL_Surface* surface, SDL_Cursor* guiCursor, int columnOffset, int editorCount, better::ConfigData config);

better::Text mouseButton(better::Text text, SDL_Event event, SDL_Surface* surface, SDL_Cursor* guiCursor, int columnOffset, int editorCount, better::ConfigData config);

better::Text mouseButtonUp(better::Text text, SDL_Event event, SDL_Surface* surface, SDL_Cursor* guiCursor, int columnOffset, int editorCount, better::ConfigData config);

better::Text mouseWheel(better::Text text, SDL_Event event, SDL_Surface* surface, SDL_Cursor* guiCursor, int columnOffset, int editorCount, better::ConfigData config);

better::Text pasteClipboard(better::Text text);

better::Text deleteHighlighted(better::Text text);

better::Text autoBracket(better::Text text, char bracket);

better::Text handleKey(better::Text text, char key);

void copyClipboard(better::Text text);

better::Text cutClipboard(better::Text text);

better::Text ctrlShortcuts(better::Text text);

char shift(char key);

char shiftLetter(char key);

char unshiftLetter(char key);

void resetMenus(bool* menus);

int selectMenu(bool menus[]);

}

#endif