#include "TextEditor.hpp"

better::datatypes::Text better::deleteHighlighted(better::datatypes::Text text, better::datatypes::Cursor cursor) { //simplify this with drops method.
    std::vector<better::datatypes::Text> tempTexts {text}; //replace all of this
    better::datatypes::Cursor endCursor {text.highlightEnd};
    cursor.column += 1;
    tempTexts.back().highlightEnd = tempTexts.back().highlightStart;
    return tempTexts.back();
}

better::datatypes::Text better::autoBracket(better::datatypes::Text text, better::datatypes::Cursor cursor, char letter) {
    int num {letter == '(' ? 1 : 2};
    return better::updateText(better::updateText(text, cursor, letter), addColumn(cursor, 1), letter + num);
}

better::datatypes::Text better::tab(better::datatypes::Text text, better::datatypes::Cursor cursor, int tabWidth) { //option for tabs as well as spaces
    immer::flex_vector_transient<char> tab{};
    for(int i{}; i < tabWidth; ++i) { //add spaces to transient vector.
        tab.push_back(' ');
    }
    immer::flex_vector<char> newLine{text.textEdit[cursor.row].take(cursor.column) + tab.persistent() + text.textEdit[cursor.row].drop(cursor.column)}; //insert the tab at the right place

    return {
        text.textEdit.set(cursor.row, newLine), //replace the line with tabs added
        text.highlightStart,
        text.highlightEnd
    };
}

int better::getPreviousIndentLevel(better::datatypes::Text text, int row) {
    int notSpace {0};
    for(int i{}; i < text.textEdit[row].size(); ++i) {
        if(text.textEdit[row][i] == ' ') {
            notSpace++;
        }
        else {
            break;
        }
    }
    return notSpace;    
}

better::datatypes::Text better::updateText(better::datatypes::Text textEdit, better::datatypes::Cursor cursor, char newChar) {
    if(newChar == '\n') {
        return textEdit;
    }
    bool endOfLine {cursor.column == textEdit.textEdit[cursor.row].size()};
    immer::flex_vector<char> line {endOfLine ? textEdit.textEdit[cursor.row].push_back(newChar) : textEdit.textEdit[cursor.row].insert(cursor.column, newChar)}; //add the character to the line (change to insert unless the cursor is at the end of the line)
    better::datatypes::Text tempText
    {
        textEdit.textEdit.set(cursor.row,line), //textEdit
        textEdit.highlightStart, //hightlightStart
        textEdit.highlightEnd //higlightEnd
    };
    return tempText;
}

better::datatypes::Text better::backspace(better::datatypes::Text text, better::datatypes::Cursor cursor) { //couple of bugs with backspace, figure out once got all chars rendering
    if(!cursor.row && !cursor.column) {
        return text;
    }

    immer::flex_vector<char> newLine;

    if(cursor.column > 0) {
        newLine = text.textEdit[cursor.row].erase(cursor.column - 1); //make case for deleting newline
    }
    else {
        //concatenate rest of line with previous line
        if(text.textEdit[cursor.row].size() > 0) {
            newLine = text.textEdit[cursor.row - 1] + text.textEdit[cursor.row];
            return
            { //Text start
                text.textEdit.erase(cursor.row).set(cursor.row - 1, newLine), //textEdit
                text.highlightStart, //highlightStart
                text.highlightEnd //highlightEnd
            }; //Text end
        }
        return
        { //Text start
            text.textEdit.erase(cursor.row), //textEdit
            text.highlightStart, //highlightStart
            text.highlightEnd //highlightEnd
        }; //Text end
    }

    return
    { //Text start
        text.textEdit.set(cursor.row, newLine), //textEdit
        text.highlightStart, //highlightStart
        text.highlightEnd //hightlightEnd
    }; //Text end
}

better::datatypes::Text better::newLine(better::datatypes::Text textEdit, better::datatypes::Cursor cursor, bool autoIndent) {
    std::vector<better::datatypes::Text> texts {};
    int tabWidth {4};

    bool endOfText {cursor.row == textEdit.textEdit.size() - 1 ? true : false};
    bool isParen {(textEdit.textEdit[cursor.row][cursor.column - 1] == '{' && textEdit.textEdit[cursor.row][cursor.column] == '}') ||
                  (textEdit.textEdit[cursor.row][cursor.column - 1] == '(' && textEdit.textEdit[cursor.row][cursor.column] == ')') ||
                  (textEdit.textEdit[cursor.row][cursor.column - 1] == '[' && textEdit.textEdit[cursor.row][cursor.column] == ']')};
    
    better::datatypes::Text newText
    {
        endOfText ? textEdit.textEdit.set(cursor.row, textEdit.textEdit[cursor.row].take(cursor.column)).push_back(textEdit.textEdit[cursor.row].drop(cursor.column)) :
                    textEdit.textEdit.insert(cursor.row + 1, textEdit.textEdit[cursor.row].drop(cursor.column)).set(cursor.row, textEdit.textEdit[cursor.row].take(cursor.column)), //textEdit
        textEdit.highlightStart, //highlightStart
        textEdit.highlightEnd //highlightEnd
    };
    
    texts.push_back(newText);
    
    if(autoIndent) {
        int prevIndent {better::getPreviousIndentLevel(textEdit, cursor.row)};
        cursor.row += 1;
        for(int i{}; i < prevIndent; ++i) {
            texts.push_back(better::updateText(texts.back(), cursor,' '));
            cursor.column += 1;
        }
    }

    cursor.column -= 1;

    if(isParen) {
        texts.push_back(better::newLine(texts.back(), cursor, true)); //create empty new line followed by brace
        texts.push_back(better::tab(texts.back(), cursor, tabWidth)); //cursor is still at the start of empty line, add an indent there
    }
    
    return texts.back();
}