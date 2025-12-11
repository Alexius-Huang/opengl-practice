#ifndef READ_SHADER_FILE_H
#define READ_SHADER_FILE_H

#include <filesystem>
namespace fs = std::filesystem;

unsigned int readShaderFile(fs::path path);

#endif // READ_SHADER_FILE_H
