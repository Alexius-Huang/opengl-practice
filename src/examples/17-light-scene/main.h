#ifndef _17_LIGHT_SCENE_H
#define _17_LIGHT_SCENE_H

#include "../includes.h"

class _17_LightScene : public Example {
private:
    unsigned int vertexShader;
    unsigned int fragmentShader;
    unsigned int lightFragmentShader;
    ShaderProgram* shaderProgram;
    ShaderProgram* lightShaderProgram;
    Texture2D* texture1;
    Texture2D* texture2;

    PerspectiveCamera* camera;

    glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
    glm::vec3 objectPosition = glm::vec3(.0f);
    glm::vec3 objectScale = glm::vec3(1.0f, 1.0, 1.0f);

    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 lightPosition = glm::vec3(1.2f, 1.0f, 2.0f);
    glm::vec3 lightScale = glm::vec3(.2f);

    bool isPressingTab = false;
    bool isListeningMouseEvent = false;

    Cube* cube;

public:
    _17_LightScene(Context* ctx) : Example("Light Scene", ctx) {}

    ~_17_LightScene() {
        delete this->shaderProgram;
        delete this->cube;
    }

    void setup() override;
    void render() override;
    void cleanup() override;
};

#endif  // _17_LIGHT_SCENE_H
