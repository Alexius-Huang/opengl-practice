#ifndef _16_CAMERA_CONTROL_H
#define _16_CAMERA_CONTROL_H

#include "../includes.h"

class _16_CameraControl : public Example {
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
    
    glm::vec3 cameraFront;
    glm::vec3 cameraPosition;

    Cube* cube;

public:
    _16_CameraControl(Context* ctx) : Example("Camera Control", ctx) {}

    ~_16_CameraControl() {
        delete this->shaderProgram;
        delete this->cube;
    }

    void setup() override;
    void render() override;
    void cleanup() override;

    void generateTransformationMatrix();
};

#endif  // _16_CAMERA_CONTROL_H
