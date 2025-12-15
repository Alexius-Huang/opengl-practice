#ifndef _15_CAMERA_LOOK_AT_MATRIX_H
#define _15_CAMERA_LOOK_AT_MATRIX_H

#include "../includes.h"

class _15_CameraLookAtMatrix : public Example {
private:
    unsigned int vertexShader;
    unsigned int fragmentShader;
    ShaderProgram* shaderProgram;
    unsigned int VAO;
    unsigned int VBO;
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
    
    const glm::vec3 cameraTarget = glm::vec3(.0f);

public:
    _15_CameraLookAtMatrix(Context* ctx) : Example("Multiple 3D Cubes", ctx) {}

    ~_15_CameraLookAtMatrix() { delete this->shaderProgram; }

    void setup() override;
    void render() override;
    void cleanup() override;

    void generateTransformationMatrix();
};

#endif  // _15_CAMERA_LOOK_AT_MATRIX_H
