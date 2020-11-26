#include "datatypes.hpp"
#include <iostream>

better::Text better::updateText(better::Text textEdit, char newChar) {
    if(newChar == '\n') { //make case for newline in middle of line
        std::cout << "newline" << std::endl;
        better::Text newText = {textEdit.textEdit.push_back(immer::flex_vector<char> {' '}), {0,textEdit.cursor.row + 1}, textEdit.topLineNumber, textEdit.topColumnNumber}; //insert newline unless at bottom
        return newText;
    } //change the topline number variable if needs to scroll
    else {
        immer::flex_vector<char> line {textEdit.textEdit[textEdit.cursor.row].push_back(newChar)}; //add the character to the line (change to insert unless the cursor is at the end of the line)
        better::Text newText {textEdit.textEdit.set(textEdit.cursor.row,line),{textEdit.cursor.row,textEdit.cursor.column + 1},textEdit.topLineNumber, textEdit.topColumnNumber};
        //create a new text struct with the updated line and updated cursor
        return newText; //return the new text
    }
}

better::Text better::backspace(better::Text text) { //find out why backspace stops working
    if(text.cursor.row == 0 && text.cursor.column == -1) {
        return text;
    }

    immer::flex_vector<char> newLine;

    if(text.cursor.column > -1) {
        newLine = text.textEdit[text.cursor.row].erase(text.cursor.column); //make case for deleting newline
    }
    else {
        //concatenate rest of line with previous line
        if(text.textEdit[text.cursor.row].size() - 1 > 1) {
            int colNo = text.textEdit[text.cursor.row - 1].size() - 1;
            newLine = text.textEdit[text.cursor.row - 1] + text.textEdit[text.cursor.row].erase(text.cursor.column);
            return {text.textEdit.erase(text.cursor.row).set(text.cursor.row - 1, newLine), {text.cursor.row - 1, colNo}, text.topLineNumber, text.topColumnNumber};
        }
        return {text.textEdit.erase(text.cursor.row), {text.cursor.row - 1, static_cast<int>(text.textEdit[text.cursor.row - 1].size() - 1)}, text.topLineNumber, text.topColumnNumber};
    }

    return {text.textEdit.set(text.cursor.row,newLine), {text.cursor.row, text.cursor.column - 1}, text.topLineNumber, text.topColumnNumber};
}

better::Cursor better::findCursorPos(int topLine, SDL_Event& event) {
    //use line height to determine relative cursor position
    //add to topline to find actual line number
    //use column width to determine column number
    return better::Cursor {};
}