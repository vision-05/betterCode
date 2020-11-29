#include "fileUtils.hpp"

int better::saveFile(immer::flex_vector<immer::flex_vector<char>> contents, char filename[]) {
    return 0;
}

immer::flex_vector<immer::flex_vector<char>> better::readFile(char filename[]) {
    immer::flex_vector_transient<immer::flex_vector<char>> textContents {};
    std::string buffer {};

    std::ifstream infile {};
    infile.open(filename);

    while(std::getline(infile, buffer)) {
        textContents.push_back(stringToVector(buffer));
        buffer = "";
    }

    if(textContents[textContents.size() - 1].size() == 1 && textContents[textContents.size() - 1][0] == '\n') {
        textContents.take(textContents.size() - 2);
    }

    return textContents.persistent();
}

immer::flex_vector<char> better::stringToVector(std::string string) {
    immer::flex_vector_transient<char> vector {};
    for (const char character: string) {
        vector.push_back(character);
    }
    return vector.persistent();
}