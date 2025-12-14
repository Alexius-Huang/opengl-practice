#ifndef _10_USING_MULTIPLE_TEXTURES_H
#define _10_USING_MULTIPLE_TEXTURES_H

#include "../includes.h"

class _10_UsingMultipleTextures : public Example {
private:
    unsigned int vertexShader;
    unsigned int fragmentShader;
    ShaderProgram* shaderProgram;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    Texture2D* texture1;
    Texture2D* texture2;

    float accumulatedTime = .0f;
    float mixPercentage = .2f;
public:
    _10_UsingMultipleTextures(Context* ctx) : Example("Using Multiple Textures", ctx) {}

    ~_10_UsingMultipleTextures() { delete this->shaderProgram; }

    void setup() override;
    void render() override;
    void cleanup() override;
};

#endif  // _10_USING_MULTIPLE_TEXTURES_H
