#ifndef _30_FACE_CULLING_H
#define _30_FACE_CULLING_H

#include "../includes.h"

class _30_FaceCulling : public Example {
private:
    unsigned int vertexShader;
    unsigned int fragmentShader;
    ShaderProgram* shaderProgram;
    Texture2D* textureCube;

    bool isPressingTab = false;

    Cube* cube;
    PerspectiveCamera* camera;

public:
    _30_FaceCulling(Context* ctx) : Example("Face Culling", ctx) {}

    ~_30_FaceCulling() {
        delete this->shaderProgram;
        delete this->cube;
    }

    void setup() override;
    void render() override;
    void cleanup() override;
};

#endif  // _30_FACE_CULLING_H
