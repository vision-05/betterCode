#include "DataTypes.hpp"

bool better::datatypes::operator==(better::datatypes::Cursor lhs, better::datatypes::Cursor rhs) {
    return (lhs.row == rhs.row) && (lhs.column == rhs.column);
}

bool better::datatypes::operator>=(better::datatypes::Cursor lhs, better::datatypes::Cursor rhs) {
    return (lhs.row > rhs.row) || (lhs.row == rhs.row && lhs.column >= rhs. column);
}

bool better::datatypes::operator<=(better::datatypes::Cursor lhs, better::datatypes::Cursor rhs) {
    return (lhs.row < rhs.row) || (lhs.row == rhs.row && lhs.column <= rhs.column);
}

better::datatypes::Cursor better::addColumn(const better::datatypes::Cursor cursor, int column) {
    return better::datatypes::Cursor {.row = cursor.row, .column = cursor.column + column};
}

better::datatypes::Cursor better::addRow(const better::datatypes::Cursor cursor, int row) {
    return better::datatypes::Cursor {.row = cursor.row + row, .column = cursor.column};
}