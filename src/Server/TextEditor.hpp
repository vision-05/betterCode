#ifndef TEXT_EDITOR_HPP
#define TEXT_EDITOR_HPP

#include <string>
#include <vector>
#include <immer-0.6.2/immer/flex_vector.hpp>

#include "DataTypes.hpp"

namespace better {
    better::datatypes::Text tab(better::datatypes::Text text, int tabWidth);

    int getPreviousIndentLevel(better::datatypes::Text text, int row);

    better::datatypes::Text updateText(better::datatypes::Text textEdit, char newChar);

    better::datatypes::Text backspace(better::datatypes::Text text);

    better::datatypes::Text newLine(better::datatypes::Text textEdit, bool autoIndent);

    better::datatypes::Text deleteHighlighted(better::datatypes::Text text);

    better::datatypes::Text moveCursorLeft(better::datatypes::Text text)

    better::datatypes::Text autoBracket(better::datatypes::Text text, char letter);
    
    immer::flex_vector<immer::flex_vector<char>> getHighlighted(better::datatypes::Text text);
}

#endif