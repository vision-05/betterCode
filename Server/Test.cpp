#include <gtest/gtest.h>

#include "FileOps.hpp"
#include "TextEditor.hpp"
#include "DataTypes.hpp"

using fv = immer::flex_vector<immer::flex_vector<char>>;

TEST(TextEditTest, InsertingCharacters) {
    EXPECT_EQ((fv{{'h','i','x','j','k','l'}}), (better::updateText({fv{{'h','i','j','k','l'}},0,0}, {0,2}, 'x').textEdit)); //insert at middle
    EXPECT_EQ((fv{{'d','f','x'}}), (better::updateText({fv{{'d','f'}},0,0}, {0,2}, 'x').textEdit)); //insert at end
    EXPECT_EQ((fv{{},{'x'}}), (better::updateText({fv{{},{}},0,0}, {1,0}, 'x').textEdit)); //insert at line start
    EXPECT_EQ((fv{{'x'}}), (better::updateText({fv{{}},0,0}, {0,0}, 'x').textEdit)); //insert at text start
    EXPECT_EQ((fv{{}}), (better::updateText({fv{{}},0,0}, {0,0}, '\n').textEdit)); //insert newline character
}

TEST(TextEditTest, InsertingNewlines) {
    EXPECT_EQ((fv{{},{}}), (better::newLine({fv{{}},0,0}, {0,0}, true).textEdit)); //newline at start of line
    EXPECT_EQ((fv{{},{'d'}}), (better::newLine({fv{{'d'}},0,0}, {0,0}, true).textEdit)); //newline at start of line with text after
    EXPECT_EQ((fv{{' ',' ',' ',' ','f'},{' ',' ',' ',' '}}), (better::newLine({fv{{' ',' ',' ',' ','f'}},0,0}, {0,5}, true).textEdit)); //test to see if previous indentation is preserved
    EXPECT_EQ((fv{{'d','d'},{'d','d'}}), (better::newLine({fv{{'d','d','d','d'}},0,0}, {0,2}, true).textEdit)); //test newline in middle of line
    EXPECT_EQ((fv{{'d','d','d'},{}}), (better::newLine({fv{{'d','d','d'}},0,0}, {0,3}, true).textEdit)); //test newline at end of line
    EXPECT_EQ((fv{{'{'},{' ',' ',' ',' '},{'}'}}), (better::newLine({fv{{'{','}'}},0,0}, {0,1}, true).textEdit)); //test newline in between braces
}

TEST(TextEditTest, Backspacing) {

}

TEST(TextEditTest, DeletingHighlighted) {
    
}

TEST(DataTypesTest, translateIndexToCursor) {
    EXPECT_EQ((better::Cursor {2,2}), (better::translateIndexToCursor(4, better::Text{fv{{'c','c'},{},{'f','f','j'}}, {0,0}, {0,0}})));
    EXPECT_EQ((better::Cursor {-1,-1}), (better::translateIndexToCursor(10, better::Text{fv{{'c','c'},{},{'f','f','j'}}, {0,0}, {0,0}})));
    EXPECT_EQ((better::Cursor {-1,-1}), (better::translateIndexToCursor(-1, better::Text{fv{{'c','c'},{},{'f','f','j'}}, {0,0}, {0,0}})));
    EXPECT_EQ((better::Cursor {0,0}), (better::translateIndexToCursor(0, better::Text{fv{{'c','c'},{},{'f','f','j'}}, {0,0}, {0,0}})));
    EXPECT_EQ((better::Cursor {3,3}), (better::translateIndexToCursor(9, better::Text{fv{{'c','c','c'},{},{'f','f','j'},{'s','s','s','s'}}, {0,0}, {0,0}})));
    EXPECT_EQ((better::Cursor {-1,-1}), (better::translateIndexToCursor(5, better::Text{fv{{'c','c'},{},{'f','f','j'}}, {0,0}, {0,0}})));
    EXPECT_EQ((better::Cursor {-1,-1}), (better::translateIndexToCursor(6, better::Text{fv{{'c','c'},{},{'f','f','j'}}, {0,0}, {0,0}})));
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}