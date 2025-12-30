#ifndef _32_POSTPROCESSING_H
#define _32_POSTPROCESSING_H

#include "../includes.h"

class _32_Postprocessing : public Example {
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

    // 1 = default
    // 2 = inversion
    // 3 = grayscale
    int uMode = 1;

    bool isPressingTab = false;
    bool isListeningMouseEvent = false;

    Cube* cube;
    Plane* floor;
    PerspectiveCamera* camera;

public:
    _32_Postprocessing(Context* ctx) : Example("Postprocessing", ctx) {}

    ~_32_Postprocessing() {
        delete this->shaderProgram;
        delete this->cube;
    }

    void setup() override;
    void render() override;
    void cleanup() override;
};

#endif  // _32_POSTPROCESSING_H
