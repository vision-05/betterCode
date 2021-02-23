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