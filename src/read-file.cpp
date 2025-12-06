#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include "./read-file.h"

std::string readFile(std::string path) {
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
