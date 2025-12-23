#ifndef _25_LOADING_MODEL_USING_ASSIMP_H
#define _25_LOADING_MODEL_USING_ASSIMP_H

#include "../includes.h"

class _25_LoadingModelUsingAssimp : public Example {
private:
    unsigned int vertexShader;
    unsigned int fragmentShader;
    ShaderProgram* shaderProgram;

    PerspectiveCamera* camera;

    bool isPressingTab = false;

    Model* model;

public:
    _25_LoadingModelUsingAssimp(Context* ctx) : Example("Loading Model Using Assimp", ctx) {}

    ~_25_LoadingModelUsingAssimp() {
        delete this->shaderProgram;
    }

    void setup() override;
    void render() override;
    void cleanup() override;
};

#endif  // _25_LOADING_MODEL_USING_ASSIMP_H
