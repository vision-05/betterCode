#include "FileOps.hpp"

immer::flex_vector<char> better::stringToVector(std::string string) {
    immer::flex_vector_transient<char> vector {};
    for (const char character : string) {
        if(character == '\t') {
            for(int i{}; i < 4; ++i) {
                vector.push_back(' ');
            }
        }
        else {
            vector.push_back(character);
        }
    }
    return vector.persistent();
}

void better::saveFile(std::string filename, immer::flex_vector<immer::flex_vector<char>> contents) {
    std::string buffer {""};
    std::ofstream file;
    file.open(filename, std::ios::out);

    for(int i{}; i < contents.size(); ++i) {
        for(int j{}; j < contents[i].size(); ++j) {
            buffer.push_back(contents[i][j]);
        }
        buffer.push_back('\n');
        file << buffer;
        buffer = "";
    }
    return;
}

immer::flex_vector<immer::flex_vector<char>> better::openFile(std::string filename) {
    immer::flex_vector_transient<immer::flex_vector<char>> textContents {};
    std::string buffer {};

    std::ifstream infile {};
    infile.open(filename);
    if(!infile) {
        textContents.push_back({});
        return textContents.persistent();
    }

    while(std::getline(infile, buffer)) {
        textContents.push_back(stringToVector(buffer));
        buffer = "";
    }

    if(textContents[textContents.size() - 1].size() == 1 && textContents[textContents.size() - 1][0] == '\n') {
        textContents.take(textContents.size() - 2);
    }
    if(textContents.size() == 0) {
        textContents.push_back({});
    }

    return textContents.persistent();
}
