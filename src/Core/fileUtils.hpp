#ifndef FILE_UTILS_HPP
#define FILE_UTILS_HPP

#include <fstream>
#include <string>
#include <immer/flex_vector.hpp>

namespace better {
//TODO: finish these function definitions to get file I/O working
int saveFile(immer::flex_vector<immer::flex_vector<char>> contents, char filename[]);
immer::flex_vector<immer::flex_vector<char>> readFile(char filename[]);
immer::flex_vector<char> stringToVector(std::string string);

}

#endif