#ifndef _05_USING_MULTIPLE_SHADERS_H
#define _05_USING_MULTIPLE_SHADERS_H

#include "../includes.h"

class _05_UsingMultipleShaders : public Example {
private:
    unsigned int vertexShader;
    unsigned int fragmentShader1;
    unsigned int fragmentShader2;
    ShaderProgram* shaderProgram1;
    ShaderProgram* shaderProgram2;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

public:
    _05_UsingMultipleShaders(Context* ctx) : Example("Using Multiple Shaders", ctx) {}

    ~_05_UsingMultipleShaders() {
        delete this->shaderProgram1;
        delete this->shaderProgram2;
    }

    void setup() override;
    void render() override;
    void cleanup() override;
};

#endif  // _05_USING_MULTIPLE_SHADERS_H
