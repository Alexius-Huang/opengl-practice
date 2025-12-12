#ifndef _06_PASS_DATA_BETWEEN_SHADERS_H
#define _06_PASS_DATA_BETWEEN_SHADERS_H

#include "../includes.h"

extern string _06_title;

void _06_passDataBetweenShaders(Context* ctx);

class _06_PassDataBetweenShaders : public Example {
private:
    unsigned int vertexShader;
    unsigned int fragmentShader;
    ShaderProgram* shaderProgram;
    unsigned int VAO;
    unsigned int VBO;

public:
    _06_PassDataBetweenShaders(Context* ctx) : Example("Passing Data Between Shaders", ctx) {}

    ~_06_PassDataBetweenShaders() { delete this->shaderProgram; }

    void setup() override;
    void render() override;
    void cleanup() override;
};

#endif  // _06_PASS_DATA_BETWEEN_SHADERS_H
