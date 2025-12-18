#ifndef _17_LIGHT_SCENE_H
#define _17_LIGHT_SCENE_H

#include "../includes.h"

class _17_LightScene : public Example {
private:
    unsigned int vertexShader;
    unsigned int fragmentShader;
    ShaderProgram* shaderProgram;
    Texture2D* texture1;
    Texture2D* texture2;

    // Transformation matrices
    glm::mat4 view;
    glm::mat4 projection;

    const glm::vec3 cubePositions[10] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };
    
    bool isPressingTab = false;

    // Camera direction depends on:
    // - pitch: rotation around x-axis (camera top-bottom direction)
    // - yaw: rotation around y-axis (camera left-right direction)
    // - roll: rotation around z-axis (direction of the camera), we do not implement this in example
    float pitch = 0;
    float yaw = -90;

    glm::vec3 cameraPosition = glm::vec3(.0f, .0f, 3.0f);

    Cube* cube;

    glm::vec3 deriveCameraFrontVector() {
        glm::vec3 direction = glm::vec3(
            cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
            sin(glm::radians(pitch)),
            sin(glm::radians(yaw)) * cos(glm::radians(pitch))
        );

        return glm::normalize(direction);
    }

public:
    _17_LightScene(Context* ctx) : Example("Light Scene", ctx) {}

    ~_17_LightScene() {
        delete this->shaderProgram;
        delete this->cube;
    }

    void setup() override;
    void render() override;
    void cleanup() override;

    void generateTransformationMatrix();
};

#endif  // _17_LIGHT_SCENE_H
