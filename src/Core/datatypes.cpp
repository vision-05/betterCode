#include "datatypes.hpp"
#include <iostream>

better::Text better::updateText(better::Text textEdit, char newChar) {
    if(newChar == '\n') { //make case for newline in middle of line
        std::cout << "newline" << std::endl;
        better::Text newText = {textEdit.textEdit.push_back(immer::flex_vector<char> {' '}), {0,textEdit.cursor.row + 1}, textEdit.topLineNumber}; //insert newline unless at bottom
        return newText;
    } //change the topline number variable if needs to scroll
    else {
        immer::flex_vector<char> line {textEdit.textEdit[textEdit.cursor.row].push_back(newChar)}; //add the character to the line (change to insert unless the cursor is at the end of the line)
        better::Text newText {textEdit.textEdit.set(textEdit.cursor.row,line),{textEdit.cursor.row,textEdit.cursor.column + 1},textEdit.topLineNumber};
        //create a new text struct with the updated line and updated cursor
        return newText; //return the new text
    }
}

better::Text better::backspace(better::Text text) { //find out why backspace stops working
    immer::flex_vector<char> newLine = text.textEdit[text.cursor.row].erase(text.cursor.column); //make case for deleting newline
    return {text.textEdit.set(text.cursor.row,newLine), text.cursor, text.topLineNumber};
}

better::Cursor better::findCursorPos(int topLine, SDL_Event& event) {
    //use line height to determine relative cursor position
    //add to topline to find actual line number
    //use column width to determine column number
    return better::Cursor {};
}