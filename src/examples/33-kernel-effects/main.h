#ifndef _33_KERNEL_EFFECTS_H
#define _33_KERNEL_EFFECTS_H

#include "../includes.h"

class _33_KernelEffects : public Example {
private:
    unsigned int vertexShader;
    unsigned int fragmentShader;
    ShaderProgram* shaderProgram;
    Texture2D* textureCube;
    Texture2D* textureFloor;

    unsigned int FBO; // frame buffer object
    unsigned int RBO; // render buffer object
    unsigned int textureColorBuffer;
    unsigned int frameBufferVertexShader;
    unsigned int frameBufferFragmentShader;
    ShaderProgram* frameBufferShaderProgram;

    // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
    const float QUAD_VERTICES[24] = {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    unsigned int quadVBO;
    unsigned int quadVAO;

    float uTexCoordOffset = 1.0f / 300.0f;

    // 1: default
    // 2: sharpening
    // 3: blur
    // 4: edge detection
    unsigned int uMode = 1;

    bool isPressingTab = false;
    bool isListeningMouseEvent = false;

    Cube* cube;
    Plane* floor;
    PerspectiveCamera* camera;

public:
    _33_KernelEffects(Context* ctx) : Example("Kernel Effects", ctx) {}

    ~_33_KernelEffects() {
        delete this->shaderProgram;
        delete this->cube;
    }

    void setup() override;
    void render() override;
    void cleanup() override;
};

#endif  // _33_KERNEL_EFFECTS_H
