#ifndef DATA_TYPES_HPP
#define DATA_TYPES_HPP

#include <vector>
#include <string>
#include <immer-0.6.2/immer/flex_vector.hpp>
//TODO: get rid of cursor as part of text

namespace better{
    
    struct Cursor { //store the line number and column number of the cursor
        int row;
        int column;
    };

    bool operator==(better::Cursor lhs, better::Cursor rhs);

    bool operator>=(better::Cursor lhs, better::Cursor rhs);

    bool operator<=(better::Cursor lhs, better::Cursor rhs);

    struct Text {
        immer::flex_vector<immer::flex_vector<char>> textEdit; //keeps the text state
        better::Cursor highlightStart {0,0};
        better::Cursor highlightEnd {0,0};
    };

    struct DataOut {
        std::string text;
        std::uint32_t size;
    };

    class DataIn {
    public:
        char* data;
        std::size_t size;

        DataIn(std::size_t s) : size{s}, data{new char[s]} {}
        ~DataIn() {
            delete data;
            data = nullptr;
        }
        std::string toString();
        std::string toParsedString();
    };

    better::Cursor addColumn(const better::Cursor cursor, int column);

    better::Cursor addRow(const better::Cursor cursor, int row);

    better::DataOut fvToString(const better::Text text);

    better::Cursor translateIndexToCursor(int index, const better::Text text);
}

#endif