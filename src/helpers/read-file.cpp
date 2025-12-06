#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <filesystem>
#include "./read-file.h"
namespace fs = std::__fs::filesystem;

std::string readFile(fs::path path) {
    std::ifstream file;
    std::stringstream stream;
    try {
        file.open(path);
        stream << file.rdbuf();
        file.close();
    } catch (std::ifstream::failure& e) {
        std::cout << "ERROR::READ_FILE::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    std::string content = stream.str();
    return content;
}
