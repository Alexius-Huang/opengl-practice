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
    void attachShader(unsigned int shader);
    void link();
    void use();
    void dispose();

    void setUniformF(const char* uniformName, float uniformValue);
    void setUniformI(const char* uniformName, int uniformValue);
    void setUniformVec3(const char* uniformName, const float* vectorValue);
    void setUniformMat4(const char* uniformName, const float* matrixValue);
};

#endif // SHADER_PROGRAM_H
