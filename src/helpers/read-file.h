#ifndef READ_FILE_H
#define READ_FILE_H

#include <string>
#include <filesystem>
namespace fs = std::filesystem;

std::string readFile(fs::path path);

#endif // READ_FILE_H