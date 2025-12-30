#ifndef _31_FRAME_BUFFER_H
#define _31_FRAME_BUFFER_H

#include "../includes.h"

class _31_FrameBuffer : public Example {
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

    bool isPressingTab = false;
    bool isListeningMouseEvent = false;

    Cube* cube;
    Plane* floor;
    PerspectiveCamera* camera;

public:
    _31_FrameBuffer(Context* ctx) : Example("Frame Buffer", ctx) {}

    ~_31_FrameBuffer() {
        delete this->shaderProgram;
        delete this->cube;
    }

    void setup() override;
    void render() override;
    void cleanup() override;
};

#endif  // _31_FRAME_BUFFER_H
