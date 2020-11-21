#ifndef FILE_UTILS_HPP
#define FILE_UTILS_HPP

#include <fstream>
#include <immer/flex_vector.hpp>

namespace better {
//TODO: finish these function definitions to get file I/O working
int saveFile(const immer::flex_vector<immer::flex_vector<char>> contents, char filename[]);
immer::flex_vector<immer::flex_vector<char>> readFile(char filename[]);

}

#endif