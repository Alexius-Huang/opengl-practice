#include <glad/glad.h>
#include <vector>
#include <string>
#include <stdexcept>
#include "shader-program.h"

void ShaderProgram::throwAlreadyLinkedError(const string& msg) {
    throw runtime_error(
        "ERROR::SHADER_PROGRAM::PROGRAM_ALREADY_LINKED\n" +
        msg
    );
}

void ShaderProgram::throwNotLinkedYetError(const string& msg) {
    throw runtime_error(
        "ERROR::SHADER_PROGRAM::PROGRAM_NOT_LINKED_YET\n" +
        msg
    );
}

ShaderProgram::ShaderProgram() {
    program = glCreateProgram();
}

ShaderProgram* ShaderProgram::attachShader(unsigned int shader) {
    if (hasAlreadyLinked) throwAlreadyLinkedError(
        "Unable to attach new shader as the shader program is already linked"
    );

    shaders.push_back(shader);
    return this;
}

ShaderProgram* ShaderProgram::link() {
    if (hasAlreadyLinked) throwAlreadyLinkedError(
        "Unable to link shader program because it is already linked"
    );

    for (size_t i = 0; i < shaders.size(); i++) { // Use size_t for loop counter
        glAttachShader(program, shaders.at(i));
    }

    glLinkProgram(program);

    int success;
    char infoLog[1024]; // Increased buffer size for infoLog
    glGetProgramiv(program, GL_LINK_STATUS, &success); // Use 'program' member variable
    if (!success) {
        glGetProgramInfoLog(program, 1024, NULL, infoLog); // Use 'program' member variable
        throw runtime_error("ERROR::SHADER_PROGRAM::LINK_FAILURE\n" + string(infoLog));
    }

    // After linking, detach the shaders
    for (size_t i = 0; i < shaders.size(); i++) {
        glDetachShader(program, shaders.at(i)); // Detach after linking
    }
    shaders.clear(); // Clear the vector

    hasAlreadyLinked = true;
    return this;
}

ShaderProgram* ShaderProgram::use() {
    if (!hasAlreadyLinked) throwNotLinkedYetError(
        "Shader program cannot be used as it is not linked yet"
    );
    glUseProgram(program);
    return this;
}

void ShaderProgram::dispose() {
    if (program != 0) { // Only delete if program was created
        glDeleteProgram(program);
        program = 0; // Set to 0 to prevent double deletion
    }
}

ShaderProgram* ShaderProgram::setUniformF(const char* uniformName, float uniformValue) {
    GLint uniformLocation = glGetUniformLocation(this->program, uniformName);
    glUniform1f(uniformLocation, uniformValue);
    return this;
}

ShaderProgram* ShaderProgram::setUniformI(const char* uniformName, int uniformValue) {
    GLint uniformLocation = glGetUniformLocation(this->program, uniformName);
    glUniform1i(uniformLocation, uniformValue);
    return this;
}

ShaderProgram* ShaderProgram::setUniformVec3(const char* uniformName, const float* vectorValue) {
    GLint uniformLocation = glGetUniformLocation(this->program, uniformName);
    glUniform3fv(uniformLocation, 1, vectorValue);
    return this;
}

ShaderProgram* ShaderProgram::setUniformMat3(const char* uniformName, const float* matrixValue) {
    GLint uniformLocation = glGetUniformLocation(this->program, uniformName);

    // Set 1 4x4 matrix, GL_FALSE indicates that the matrix should not transpose
    glUniformMatrix3fv(uniformLocation, 1, GL_FALSE, matrixValue);
    return this;
}

ShaderProgram* ShaderProgram::setUniformMat4(const char* uniformName, const float* matrixValue) {
    GLint uniformLocation = glGetUniformLocation(this->program, uniformName);

    // Set 1 4x4 matrix, GL_FALSE indicates that the matrix should not transpose
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, matrixValue);
    return this;
}
