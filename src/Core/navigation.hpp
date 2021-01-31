#ifndef NAVIGATION_HPP
#define NAVIGATION_HPP

#include <SDL2/SDL.h>
#include <immer-0.6.2/immer/flex_vector.hpp>

#include "datatypes.hpp"

namespace better {

better::Text verticalNav(better::Text text, SDL_Keycode key);
better::Text horizontalNav(better::Text text, SDL_Keycode key);
better::Text upNav(better::Text text);
better::Text downNav(better::Text text);
better::Text leftNav(better::Text text);
better::Text rightNav(better::Text text);

better::Text scroll(better::Text text, SDL_Event event);
better::Text scrollUp(better::Text text, int y, int row);
better::Text scrollDown(better::Text text, int y, int row);
better::Text scrollLeft(better::Text text, int x, int column);
better::Text scrollRight(better::Text text, int x, int column, int row);

}

#endif