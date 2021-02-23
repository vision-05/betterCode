#ifndef DATA_TYPES_HPP
#define DATA_TYPES_HPP

#include <vector>
#include <string>
#include <immer-0.6.2/immer/flex_vector.hpp>
//TODO: get rid of cursor as part of text

namespace better::datatypes {
    
    struct Cursor { //store the line number and column number of the cursor
        int row;
        int column;
    };

    bool operator==(better::datatypes::Cursor lhs, better::datatypes::Cursor rhs);

    bool operator>=(better::datatypes::Cursor lhs, better::datatypes::Cursor rhs);

    bool operator<=(better::datatypes::Cursor lhs, better::datatypes::Cursor rhs);

    struct Text {
        immer::flex_vector<immer::flex_vector<char>> textEdit; //keeps the text state
        better::datatypes::Cursor cursor; //keeps the cursor state
        better::datatypes::Cursor highlightStart {0,0};
        better::datatypes::Cursor highlightEnd {0,0};
    };
}

#endif