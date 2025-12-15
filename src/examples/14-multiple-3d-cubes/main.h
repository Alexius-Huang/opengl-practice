#ifndef _14_MULTIPLE_3D_CUBES_H
#define _14_MULTIPLE_3D_CUBES_H

#include "../includes.h"

class _14_Multiple3DCubes : public Example {
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

    float x = 0.0f;
    float y = 0.0f;

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

public:
    _14_Multiple3DCubes(Context* ctx) : Example("Multiple 3D Cubes", ctx) {}

    ~_14_Multiple3DCubes() { delete this->shaderProgram; }

    void setup() override;
    void render() override;
    void cleanup() override;

    void generateTransformationMatrix();

    void translateOnWASDKeyPressed();
};

#endif  // _14_MULTIPLE_3D_CUBES_H
