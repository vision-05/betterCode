#ifndef TEXT_EDITOR_HPP
#define TEXT_EDITOR_HPP

#include <string>
#include <vector>
#include <immer-0.6.2/immer/flex_vector.hpp>
#include <immer-0.6.2/immer/flex_vector_transient.hpp>

#include "DataTypes.hpp"

namespace better {
    better::Text tab(better::Text text, better::Cursor cursor, int tabWidth);

    int getPreviousIndentLevel(better::Text text, int row);

    better::Text updateText(better::Text textEdit, better::Cursor cursor, char newChar);

    better::Text backspace(better::Text text, better::Cursor cursor);

    better::Text newLine(better::Text textEdit, better::Cursor cursor, bool autoIndent);

    better::Text deleteHighlighted(better::Text text, better::Cursor cursor);

    better::Text autoBracket(better::Text text, better::Cursor cursor, char letter);
    
    immer::flex_vector<immer::flex_vector<char>> getHighlighted(better::Text text);
}

#endif