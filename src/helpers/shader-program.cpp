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

void ShaderProgram::attachShader(unsigned int shader) {
    if (hasAlreadyLinked) throwAlreadyLinkedError(
        "Unable to attach new shader as the shader program is already linked"
    );

    shaders.push_back(shader);
}

void ShaderProgram::link() {
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
}

void ShaderProgram::use() {
    if (!hasAlreadyLinked) throwNotLinkedYetError(
        "Shader program cannot be used as it is not linked yet"
    );
    glUseProgram(program);
}

void ShaderProgram::dispose() {
    if (program != 0) { // Only delete if program was created
        glDeleteProgram(program);
        program = 0; // Set to 0 to prevent double deletion
    }
}

void ShaderProgram::setUniformF(const char* uniformName, float uniformValue) {
    GLint uniformLocation = glGetUniformLocation(this->program, uniformName);
    glUniform1f(uniformLocation, uniformValue);
}
