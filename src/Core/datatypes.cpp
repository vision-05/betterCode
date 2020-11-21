#include "datatypes.hpp"

const Text updateText(const Text textEdit, char newChar) { 
    const immer::flex_vector<char> line {textEdit.textEdit[textEdit.cursor.yPos].push_back(newChar)}; //add the character to the line
    const Text newText {textEdit.textEdit.set(textEdit.cursor.yPos,line),{textEdit.cursor.yPos,textEdit.cursor.xPos + 1},textEdit.topLineNumber};
    //create a new text struct with the updated line and updated cursor
    return newText; //return the new text
}