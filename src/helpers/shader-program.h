#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <glad/glad.h>
#include <vector>
#include <string>
#include <stdexcept>

class ShaderProgram {
private:
    std::vector<unsigned int> shaders;
    unsigned int program;
    bool hasAlreadyLinked = false;

    void throwAlreadyLinkedError(const std::string& msg);
    void throwNotLinkedYetError(const std::string& msg);

public:
    ShaderProgram();
    void attachShader(unsigned int shader);
    void link();
    void use();
    void dispose();
};

#endif // SHADER_PROGRAM_H
