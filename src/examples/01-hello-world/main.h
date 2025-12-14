#ifndef _01_HELLO_WORLD_H
#define _01_HELLO_WORLD_H

#include "../includes.h"

class _01_HelloWorld : public Example {
private:
    unsigned int vertexShader;
    unsigned int fragmentShader;
    ShaderProgram* shaderProgram;
    unsigned int VAO;
    unsigned int VBO;

public:
    _01_HelloWorld(Context* ctx): Example("Hello World", ctx) {}

    ~_01_HelloWorld() {
        delete this->shaderProgram;
    }

    void setup() override;
    void render() override;
    void cleanup() override;
};

#endif // _01_HELLO_WORLD_H
