#ifndef _07_PASS_DATA_USING_VERTEX_ATTRIBUTES_H
#define _07_PASS_DATA_USING_VERTEX_ATTRIBUTES_H

#include "../includes.h"

class _07_PassDataUsingVertexAttributes : public Example {
private:
    unsigned int vertexShader;
    unsigned int fragmentShader;
    ShaderProgram* shaderProgram;
    unsigned int VAO;
    unsigned int VBO;

public:
    _07_PassDataUsingVertexAttributes(Context* ctx)
        : Example("Passing Data Using Vertex Attributes", ctx) {}

    ~_07_PassDataUsingVertexAttributes() { delete this->shaderProgram; }

    void setup() override;
    void render() override;
    void cleanup() override;
};

#endif  // _07_PASS_DATA_USING_VERTEX_ATTRIBUTES_H
