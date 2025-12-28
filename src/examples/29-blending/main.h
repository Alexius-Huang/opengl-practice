#ifndef _29_BLENDING_H
#define _29_BLENDING_H

#include "../includes.h"

class _29_Blending : public Example {
private:
    unsigned int vertexShader;
    unsigned int fragmentShader;
    ShaderProgram* shaderProgram;
    Texture2D* textureCube;
    Texture2D* textureFloor;
    Texture2D* textureWindow;

    const std::vector<glm::vec3> windowPositions {
        glm::vec3(-1.51f,  0.0f, -0.98f),
        glm::vec3( 1.49f,  0.0f,  0.01f),
        glm::vec3(-0.01f,  0.0f,  0.2f),
        glm::vec3(-0.31f,  0.0f, -2.8f),
        glm::vec3( 0.49f,  0.0f, -1.1f)
    };

    bool isPressingTab = false;

    Cube* cube;
    Plane* floor;
    Plane* window;
    PerspectiveCamera* camera;

public:
    _29_Blending(Context* ctx) : Example("Blending", ctx) {}

    ~_29_Blending() {
        delete this->shaderProgram;
        delete this->cube;
    }

    void setup() override;
    void render() override;
    void cleanup() override;
};

#endif  // _29_BLENDING_H
