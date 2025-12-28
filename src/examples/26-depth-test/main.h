#ifndef _26_DEPTH_TEST_H
#define _26_DEPTH_TEST_H

#include "../includes.h"

class _26_DepthTest : public Example {
private:
    unsigned int vertexShader;
    unsigned int fragmentShader;
    ShaderProgram* shaderProgram;
    Texture2D* textureCube;
    Texture2D* textureFloor;

    bool isPressingTab = false;

    // 1: default mode
    // 2: visualize depth buffer
    // 3: visualize linearized depth buffer
    // 4: custom, the deeper the object, it faded into background
    int depthMode = 1;

    Cube* cube;
    Plane* floor;
    PerspectiveCamera* camera;

public:
    _26_DepthTest(Context* ctx) : Example("Depth Test", ctx) {}

    ~_26_DepthTest() {
        delete this->shaderProgram;
        delete this->cube;
    }

    void setup() override;
    void render() override;
    void cleanup() override;
};

#endif  // _26_DEPTH_TEST_H
