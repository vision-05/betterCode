#ifndef FILE_OPS_HPP
#define FILE_OPS_HPP

#include <filesystem>
#include <vector>
#include <string>
#include <optional>
#include <fstream>
#include <immer-0.6.2/immer/flex_vector.hpp>
#include <immer-0.6.2/immer/flex_vector_transient.hpp>

namespace better {
    immer::flex_vector<char> stringToVector(std::string string);

    void saveFile(std::string filename, immer::flex_vector<immer::flex_vector<char>> contents);

    immer::flex_vector<immer::flex_vector<char>> openFile(std::string filename);

    std::vector<std::string> getDirectoryFiles(std::optional<std::string> workingDir = std::nullopt);

    std::vector<std::string> getDirectoryDirectories(std::optional<std::string> workingDir = std::nullopt);
}

#endif