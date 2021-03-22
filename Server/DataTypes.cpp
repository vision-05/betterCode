#include "DataTypes.hpp"

bool better::operator==(better::Cursor lhs, better::Cursor rhs) {
    return (lhs.row == rhs.row) && (lhs.column == rhs.column);
}

bool better::operator>=(better::Cursor lhs, better::Cursor rhs) {
    return (lhs.row > rhs.row) || (lhs.row == rhs.row && lhs.column >= rhs. column);
}

bool better::operator<=(better::Cursor lhs, better::Cursor rhs) {
    return (lhs.row < rhs.row) || (lhs.row == rhs.row && lhs.column <= rhs.column);
}

better::Cursor better::addColumn(const better::Cursor cursor, int column) {
    return better::Cursor {.row = cursor.row, .column = cursor.column + column};
}

better::Cursor better::addRow(const better::Cursor cursor, int row) {
    return better::Cursor {.row = cursor.row + row, .column = cursor.column};
}

better::DataOut better::fvToString(const better::Text text) {
    int textSize {};
    std::string stringText {};
    for(const immer::flex_vector<char> line : text.textEdit) {
        textSize += line.size();
        for(int i{}; i < line.size(); ++i) {
            stringText.push_back(line[i]);
        }
        stringText.push_back('\n');
    }
    return better::DataOut {.text = stringText, .size = textSize};
}

std::string better::DataIn::toString() {
    return std::string{this->data, this->data + this->size};
}

std::string better::DataIn::toParsedString() {
    return std::string{this->data + 1, this->data + this->size - 1};
}

better::Cursor better::translateIndexToCursor(int index, const better::Text text) {;
    immer::flex_vector<immer::flex_vector<char>> buffer {text.textEdit};
    int lineGreatest {0};

    for(int i{}; i < buffer.size(); ++i) {
        if(index >= lineGreatest + buffer[i].size()) {
            lineGreatest += buffer[i].size();
        }
        else {
            index -= lineGreatest;
            return better::Cursor {.row = i, .column = index};
        }
    }
    return better::Cursor {.row = -1, .column = -1};
}