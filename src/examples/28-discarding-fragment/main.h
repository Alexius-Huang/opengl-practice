#ifndef _28_DISCARDING_FRAGMENT_H
#define _28_DISCARDING_FRAGMENT_H

#include "../includes.h"

class _28_DiscardingFragment : public Example {
private:
    unsigned int vertexShader;
    unsigned int fragmentShader;
    ShaderProgram* shaderProgram;
    Texture2D* textureCube;
    Texture2D* textureFloor;
    Texture2D* textureGrass;

    const std::vector<glm::vec3> grassPositions {
        glm::vec3(-1.51f,  0.0f, -0.98f),
        glm::vec3( 1.49f,  0.0f,  0.01f),
        glm::vec3(-0.01f,  0.0f,  0.2f),
        glm::vec3(-0.31f,  0.0f, -2.8f),
        glm::vec3( 0.49f,  0.0f, -1.1f)
    };

    bool isPressingTab = false;
    bool isListeningMouseEvent = false;

    Cube* cube;
    Plane* floor;
    Plane* grass;
    PerspectiveCamera* camera;

public:
    _28_DiscardingFragment(Context* ctx) : Example("Discarding Fragment", ctx) {}

    ~_28_DiscardingFragment() {
        delete this->shaderProgram;
        delete this->cube;
    }

    void setup() override;
    void render() override;
    void cleanup() override;
};

#endif  // _28_DISCARDING_FRAGMENT_H
