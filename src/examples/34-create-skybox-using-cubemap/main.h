#ifndef _34_CREATE_SKYBOX_USING_CUBEMAP_H
#define _34_CREATE_SKYBOX_USING_CUBEMAP_H

#include "../includes.h"

class _34_CreateSkyboxUsingCubemap : public Example {
private:
    unsigned int vertexShader;
    unsigned int fragmentShader;
    ShaderProgram* shaderProgram;
    Texture2D* texture1;
    Texture2D* texture2;

    PerspectiveCamera* camera;

    glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
    glm::vec3 objectPosition = glm::vec3(.0f);
    glm::vec3 objectScale = glm::vec3(1.0f, 1.0, 1.0f);

    bool isPressingTab = false;
    bool isListeningMouseEvent = false;

    Cube* cube;

public:
    _34_CreateSkyboxUsingCubemap(Context* ctx) : Example("Create Skybox Using Cubemap", ctx) {}

    ~_34_CreateSkyboxUsingCubemap() {}

    void setup() override;
    void render() override;
    void cleanup() override;
};

#endif  // _34_CREATE_SKYBOX_USING_CUBEMAP_H
