#ifndef _09_USING_TEXTURE_H
#define _09_USING_TEXTURE_H

#include "../includes.h"

extern string _09_title;

void _09_usingTexture(Context* ctx);

class _09_UsingTexture : public Example {
private:
    unsigned int vertexShader;
    unsigned int fragmentShader;
    ShaderProgram* shaderProgram;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    unsigned int texture;

public:
    _09_UsingTexture(Context* ctx) : Example("Using Texture", ctx) {}

    ~_09_UsingTexture() { delete this->shaderProgram; }

    void setup() override;
    void render() override;
    void cleanup() override;
};

#endif  // _09_USING_TEXTURE_H
