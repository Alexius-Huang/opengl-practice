#ifndef _12_COORDINATE_SYSTEM_H
#define _12_COORDINATE_SYSTEM_H

#include "../includes.h"

class _12_CoordinateSystem : public Example {
private:
    unsigned int vertexShader;
    unsigned int fragmentShader;
    ShaderProgram* shaderProgram;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    Texture2D* texture1;
    Texture2D* texture2;

    // Transformation matrices
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;

    float x = 0.0f;
    float y = 0.0f;

public:
    _12_CoordinateSystem(Context* ctx) : Example("Coordinate System", ctx) {}

    ~_12_CoordinateSystem() { delete this->shaderProgram; }

    void setup() override;
    void render() override;
    void cleanup() override;

    void generateTransformationMatrix();

    void translateOnWASDKeyPressed();
};

#endif  // _12_COORDINATE_SYSTEM_H
