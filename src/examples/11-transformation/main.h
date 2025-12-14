#ifndef _11_TRANSFORMATION_H
#define _11_TRANSFORMATION_H

#include "../includes.h"

class _11_Transformation : public Example {
private:
    unsigned int vertexShader;
    unsigned int fragmentShader;
    ShaderProgram* shaderProgram;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    Texture2D* texture1;
    Texture2D* texture2;

    // Transformation matrix
    glm::mat4 transform;

public:
    _11_Transformation(Context* ctx) : Example("Transformation", ctx) {}

    ~_11_Transformation() { delete this->shaderProgram; }

    void setup() override;
    void render() override;
    void cleanup() override;
};

#endif  // _11_TRANSFORMATION_H
