#ifndef _13_3D_CUBE_H
#define _13_3D_CUBE_H

#include "../includes.h"

class _13_3DCube : public Example {
private:
    unsigned int vertexShader;
    unsigned int fragmentShader;
    ShaderProgram* shaderProgram;
    Texture2D* texture1;
    Texture2D* texture2;

    // Transformation matrices
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;

    float x = 0.0f;
    float y = 0.0f;

    Cube* cube;

public:
    _13_3DCube(Context* ctx) : Example("3D Cube", ctx) {}

    ~_13_3DCube() {
        delete this->shaderProgram;
        delete this->cube;
    }

    void setup() override;
    void render() override;
    void cleanup() override;

    void generateTransformationMatrix();

    void translateOnWASDKeyPressed();
};

#endif  // _13_3D_CUBE_H
