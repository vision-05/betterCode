#include "datatypes.hpp"

const better::Text better::updateText(const better::Text textEdit, char newChar) { 
    const immer::flex_vector<char> line {textEdit.textEdit[textEdit.cursor.row].push_back(newChar)}; //add the character to the line
    const better::Text newText {textEdit.textEdit.set(textEdit.cursor.row,line),{textEdit.cursor.row,textEdit.cursor.column + 1},textEdit.topLineNumber};
    //create a new text struct with the updated line and updated cursor
    return newText; //return the new text
}

better::Cursor better::findCursorPos(const int topLine, SDL_Event& event) {
    //use line height to determine relative cursor position
    //add to topline to find actual line number
    //use column width to determine column number
    return better::Cursor {};
}