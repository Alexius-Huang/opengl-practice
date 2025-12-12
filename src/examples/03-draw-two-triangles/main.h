#ifndef _03_DRAW_TWO_TRIANGLES_H
#define _03_DRAW_TWO_TRIANGLES_H

#include "../includes.h"

class _03_DrawTwoTriangles : public Example {
private:
    unsigned int vertexShader;
    unsigned int fragmentShader;
    ShaderProgram* shaderProgram;
    unsigned int VAO;
    unsigned int VBO;

public:
    _03_DrawTwoTriangles(Context* ctx) : Example("Draw Two Triangles", ctx) {}

    ~_03_DrawTwoTriangles() { delete this->shaderProgram; }

    void setup() override;
    void render() override;
    void cleanup() override;
};

#endif  // _03_DRAW_TWO_TRIANGLES_H
