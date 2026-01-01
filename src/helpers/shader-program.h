#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <glad/glad.h>
#include <vector>
#include <string>
#include <stdexcept>

using namespace std;

class ShaderProgram {
private:
    vector<unsigned int> shaders;
    unsigned int program;
    bool hasAlreadyLinked = false;

    void throwAlreadyLinkedError(const string& msg);
    void throwNotLinkedYetError(const string& msg);

public:
    ShaderProgram();
    ShaderProgram* attachShader(unsigned int shader);
    ShaderProgram* link();
    ShaderProgram* use();
    void dispose();

    ShaderProgram* setUniformF(const char* uniformName, float uniformValue);
    ShaderProgram* setUniformI(const char* uniformName, int uniformValue);
    ShaderProgram* setUniformVec2(const char* uniformName, const float* vectorValue);
    ShaderProgram* setUniformVec3(const char* uniformName, const float* vectorValue);
    ShaderProgram* setUniformMat3(const char* uniformName, const float* matrixValue);
    ShaderProgram* setUniformMat4(const char* uniformName, const float* matrixValue);
};

#endif // SHADER_PROGRAM_H
