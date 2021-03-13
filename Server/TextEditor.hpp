#ifndef TEXT_EDITOR_HPP
#define TEXT_EDITOR_HPP

#include <string>
#include <vector>
#include <immer-0.6.2/immer/flex_vector.hpp>
#include <immer-0.6.2/immer/flex_vector_transient.hpp>

#include "DataTypes.hpp"

namespace better {
    better::datatypes::Text tab(better::datatypes::Text text, better::datatypes::Cursor cursor, int tabWidth);

    int getPreviousIndentLevel(better::datatypes::Text text, int row);

    better::datatypes::Text updateText(better::datatypes::Text textEdit, better::datatypes::Cursor cursor, char newChar);

    better::datatypes::Text backspace(better::datatypes::Text text, better::datatypes::Cursor cursor);

    better::datatypes::Text newLine(better::datatypes::Text textEdit, better::datatypes::Cursor cursor, bool autoIndent);

    better::datatypes::Text deleteHighlighted(better::datatypes::Text text, better::datatypes::Cursor cursor);

    better::datatypes::Text autoBracket(better::datatypes::Text text, better::datatypes::Cursor cursor, char letter);
    
    immer::flex_vector<immer::flex_vector<char>> getHighlighted(better::datatypes::Text text);
}

#endif