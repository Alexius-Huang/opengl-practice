#ifndef _04_USING_MULTIPLE_VAO_AND_VBO_H
#define _04_USING_MULTIPLE_VAO_AND_VBO_H

#include "../includes.h"

class _04_UsingMultipleVAOAndVBO : public Example {
private:
    unsigned int vertexShader;
    unsigned int fragmentShader;
    ShaderProgram* shaderProgram;
    unsigned int VAOs[2];
    unsigned int VBOs[2];

public:
    _04_UsingMultipleVAOAndVBO(Context* ctx) : Example("Using Multiple VAOs and VBOs", ctx) {}

    ~_04_UsingMultipleVAOAndVBO() { delete this->shaderProgram; }

    void setup() override;
    void render() override;
    void cleanup() override;
};

#endif  // _04_USING_MULTIPLE_VAO_AND_VBO_H
