#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
// #include <iostream>
#include <filesystem>
#include "./read-file.h"
#include "./read-shader-file.h"

namespace fs = std::__fs::filesystem;

unsigned int readShaderFile(fs::path path) {
    std::string fileExtension = path.extension();

    unsigned int shaderType;
    if (fileExtension == ".vert") {
        shaderType = GL_VERTEX_SHADER;
    } else if (fileExtension == ".frag") {
        shaderType = GL_FRAGMENT_SHADER;
    } else {
        std::string err = "Unrecognized shader file extension: ";
        throw std::runtime_error(err + fileExtension);
    }

    std::string fileContent = readFile(path);
    const char* shaderSource = fileContent.c_str();

    unsigned int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[255];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);

        std::string shaderName = shaderType == GL_VERTEX_SHADER
            ? "VERTEX"
            : "FRAGMENT";

        throw std::runtime_error(
            "ERROR::SHADER::" + shaderName + "::COMPILATION_FAILED\n" + infoLog
        );
    }

    return shader;
}