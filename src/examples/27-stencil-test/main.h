#ifndef _27_STENCIL_TEST_H
#define _27_STENCIL_TEST_H

#include "../includes.h"

class _27_StencilTest : public Example {
private:
    unsigned int vertexShader;
    unsigned int fragmentShader;
    unsigned int stencilFragmentShader;
    ShaderProgram* shaderProgram;
    ShaderProgram* stencilShaderProgram;
    Texture2D* textureCube;
    Texture2D* textureFloor;

    bool isPressingTab = false;

    Cube* cube;
    Plane* floor;
    PerspectiveCamera* camera;

public:
    _27_StencilTest(Context* ctx) : Example("Stencil Test", ctx) {}

    ~_27_StencilTest() {
        delete this->shaderProgram;
        delete this->cube;
    }

    void setup() override;
    void render() override;
    void cleanup() override;
};

#endif  // _27_STENCIL_TEST_H
