#include "TextEditor.hpp"

better::datatypes::Text better::deleteHighlighted(better::datatypes::Text text) {
    std::vector<better::datatypes::Text> tempTexts {text};
    tempTexts.back().cursor = text.highlightEnd;
    tempTexts.back().cursor.column += 1;
    int stopColumn {0};
    for(int i{tempTexts.back().cursor.row}; i >= tempTexts.back().highlightStart.row; --i) {
        if(i == tempTexts.back().highlightStart.row) {
            stopColumn = tempTexts.back().highlightStart.column + 1;
        }
        for(int j{tempTexts.back().cursor.column}; j >= stopColumn; --j) {
            tempTexts.push_back(better::backspace(tempTexts.back()));
        }
    }
    tempTexts.back().highlightEnd = tempTexts.back().highlightStart;
    return tempTexts.back();
}

better::datatypes::Text better::moveCursorLeft(better::datatypes::Text text) {
    text.cursor.column -= 1;
    return text;
}

better::datatypes::Text better::autoBracket(better::datatypes::Text text, char letter) {
    int num {letter == '(' ? 1 : 2};
    return better::moveCursorLeft(better::updateText(better::updateText(text,letter),letter + num)); //left not defined yet
}

better::datatypes::Text better::tab(better::datatypes::Text text, int tabWidth) {
    return better::updateText(better::updateText(better::updateText(better::updateText(text,' '),' '),' '),' ');
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

better::datatypes::Text better::updateText(better::datatypes::Text textEdit, char newChar) {
    bool endOfLine {textEdit.cursor.column == textEdit.textEdit[textEdit.cursor.row].size()};
    immer::flex_vector<char> line {endOfLine ? textEdit.textEdit[textEdit.cursor.row].push_back(newChar) : textEdit.textEdit[textEdit.cursor.row].insert(textEdit.cursor.column, newChar)}; //add the character to the line (change to insert unless the cursor is at the end of the line)
    better::datatypes::Text tempText
    {
        textEdit.textEdit.set(textEdit.cursor.row,line), //textEdit
        {textEdit.cursor.row, textEdit.cursor.column + 1}, //cursor
        textEdit.highlightStart, //hightlightStart
        textEdit.highlightEnd //higlightEnd
    };
    return tempText;
}

better::datatypes::Text better::backspace(better::datatypes::Text text) { //couple of bugs with backspace, figure out once got all chars rendering
    if(!text.cursor.row && !text.cursor.column) {
        return text;
    }

    immer::flex_vector<char> newLine;

    if(text.cursor.column > 0) {
        newLine = text.textEdit[text.cursor.row].erase(text.cursor.column - 1); //make case for deleting newline
    }
    else {
        //concatenate rest of line with previous line
        if(text.textEdit[text.cursor.row].size() > 0) {
            int colNo = text.textEdit[text.cursor.row - 1].size();
            newLine = text.textEdit[text.cursor.row - 1] + text.textEdit[text.cursor.row];
            return
            { //Text start
                text.textEdit.erase(text.cursor.row).set(text.cursor.row - 1, newLine), //textEdit
                {text.cursor.row - 1, colNo}, //cursor
                text.highlightStart, //highlightStart
                text.highlightEnd //highlightEnd
            }; //Text end
        }
        return
        { //Text start
            text.textEdit.erase(text.cursor.row), //textEdit
            {text.cursor.row - 1, static_cast<int>(text.textEdit[text.cursor.row - 1].size())}, //cursor
            text.highlightStart, //highlightStart
            text.highlightEnd //highlightEnd
        }; //Text end
    }

    return
    { //Text start
        text.textEdit.set(text.cursor.row,newLine), //textEdit
        {text.cursor.row, text.cursor.column - 1}, //cursor
        text.highlightStart, //highlightStart
        text.highlightEnd //hightlightEnd
    }; //Text end
}

better::datatypes::Text better::newLine(better::datatypes::Text textEdit, bool autoIndent) { //create cases for newline inbetween brackets.
    std::vector<better::datatypes::Text> texts {};

    bool endOfText {textEdit.cursor.row == textEdit.textEdit.size() - 1 ? true : false};
    
    better::datatypes::Text newText
    {
        endOfText ? textEdit.textEdit.set(textEdit.cursor.row, textEdit.textEdit[textEdit.cursor.row].take(textEdit.cursor.column)).push_back(textEdit.textEdit[textEdit.cursor.row].drop(textEdit.cursor.column)) :
                    textEdit.textEdit.insert(textEdit.cursor.row + 1, textEdit.textEdit[textEdit.cursor.row].drop(textEdit.cursor.column)).set(textEdit.cursor.row, textEdit.textEdit[textEdit.cursor.row].take(textEdit.cursor.column)), //textEdit
        {textEdit.cursor.row + 1, 0}, //cursor
        textEdit.highlightStart, //highlightStart
        textEdit.highlightEnd //highlightEnd
    };
    
    texts.push_back(newText);
    
    if(autoIndent) {
        int prevIndent {better::getPreviousIndentLevel(textEdit, textEdit.cursor.row)};
        for(int i{}; i < prevIndent; ++i) {
            texts.push_back(better::updateText(texts.back(),' '));
        }
    }

    if(false) {
        texts.push_back(better::newline(texts.back()));
        texts.back().cursor.row -= 1;
        texts.back().cursor.column = texts.back().textEdit[texts.back().cursor.row].size();
    }
    
    return texts.back();
}