#ifndef _24_SPOT_LIGHT_H
#define _24_SPOT_LIGHT_H

#include "../includes.h"

class _24_SpotLight : public Example {
private:
    unsigned int vertexShader;
    unsigned int fragmentShader;
    ShaderProgram* shaderProgram;

    PerspectiveCamera* camera;

    glm::vec3 objectScale = glm::vec3(1.0f, 1.0, 1.0f);

    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 lightScale = glm::vec3(.2f);
    glm::vec3 lightAttenuation = glm::vec3(1.0f, .09f, .032f);

    bool isPressingTab = false;

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

    Cube* cube;

    Texture2D* diffuseMap;
    Texture2D* specularMap;

public:
    _24_SpotLight(Context* ctx) : Example("Spot Light", ctx) {}

    ~_24_SpotLight() {
        delete this->shaderProgram;
        delete this->cube;
    }

    void setup() override;
    void render() override;
    void cleanup() override;
};

#endif  // _24_SPOT_LIGHT_H
