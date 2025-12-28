#ifndef _26_DEPTH_TEST_H
#define _26_DEPTH_TEST_H

#include "../includes.h"

class _26_DepthTest : public Example {
private:
    unsigned int vertexShader;
    unsigned int fragmentShader;
    ShaderProgram* shaderProgram;
    Texture2D* textureCube;
    Texture2D* textureFloor;

    float planeVertices[30] = {
        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  2.0f, 2.0f								
    };

    unsigned int floorVAO;
    unsigned int floorVBO;

    bool isPressingTab = false;

    // Camera direction depends on:
    // - pitch: rotation around x-axis (camera top-bottom direction)
    // - yaw: rotation around y-axis (camera left-right direction)
    // - roll: rotation around z-axis (direction of the camera), we do not implement this in example
    float pitch = 0;
    float yaw = -90;

    glm::vec3 cameraPosition = glm::vec3(.0f, .0f, 3.0f);

    Cube* cube;
    PerspectiveCamera* camera;

    glm::vec3 deriveCameraFrontVector() {
        glm::vec3 direction = glm::vec3(
            cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
            sin(glm::radians(pitch)),
            sin(glm::radians(yaw)) * cos(glm::radians(pitch))
        );

        return glm::normalize(direction);
    }

public:
    _26_DepthTest(Context* ctx) : Example("Depth Test", ctx) {}

    ~_26_DepthTest() {
        delete this->shaderProgram;
        delete this->cube;
    }

    void setup() override;
    void render() override;
    void cleanup() override;
};

#endif  // _26_DEPTH_TEST_H
