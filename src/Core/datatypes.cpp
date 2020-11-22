#include "datatypes.hpp"

better::Text better::updateText(better::Text textEdit, char newChar) { 
    immer::flex_vector<char> line {textEdit.textEdit[textEdit.cursor.row].push_back(newChar)}; //add the character to the line
    better::Text newText {textEdit.textEdit.set(textEdit.cursor.row,line),{textEdit.cursor.row,textEdit.cursor.column + 1},textEdit.topLineNumber};
    //create a new text struct with the updated line and updated cursor
    if(newChar == '\n') {
        newText = {newText.textEdit.push_back(immer::flex_vector<char> {}), {0,newText.cursor.row + 1}, newText.topLineNumber};
    } //change the topline number variable if needs to scroll
    
    return newText; //return the new text
}

better::Text better::backspace(better::Text text) { //find out why backspace stops working
    immer::flex_vector<char> newLine = text.textEdit[text.cursor.row].erase(text.cursor.column);
    return {text.textEdit.set(text.cursor.row,newLine), text.cursor, text.topLineNumber};
}

better::Cursor better::findCursorPos(int topLine, SDL_Event& event) {
    //use line height to determine relative cursor position
    //add to topline to find actual line number
    //use column width to determine column number
    return better::Cursor {};
}