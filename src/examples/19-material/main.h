#ifndef _19_MATERIAL_H
#define _19_MATERIAL_H

#include "../includes.h"

class _19_Material : public Example {
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
    _19_Material(Context* ctx) : Example("Material", ctx) {}

    ~_19_Material() {
        delete this->shaderProgram;
        delete this->cube;
    }

    void setup() override;
    void render() override;
    void cleanup() override;
};

#endif  // _19_MATERIAL_H
