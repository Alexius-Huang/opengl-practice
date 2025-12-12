#ifndef _08_PASS_DATA_USING_UNIFORMS_H
#define _08_PASS_DATA_USING_UNIFORMS_H

#include "../includes.h"

extern string _08_title;

void _08_passDataUsingUniforms(Context* ctx);

class _08_PassDataUsingUniforms : public Example {
private:
    unsigned int vertexShader;
    unsigned int fragmentShader;
    ShaderProgram* shaderProgram;
    unsigned int VAO;
    unsigned int VBO;

public:
    _08_PassDataUsingUniforms(Context* ctx) : Example("Passing Data Using Uniforms", ctx) {}

    ~_08_PassDataUsingUniforms() { delete this->shaderProgram; }

    void setup() override;
    void render() override;
    void cleanup() override;
};

#endif  // _08_PASS_DATA_USING_UNIFORMS_H
