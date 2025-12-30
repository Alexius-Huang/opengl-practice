#include "main.h"
namespace _17_Config {
    float ambient = .2f;
    float specularStength = .5f;
    int specularPower = 32;
}

void _17_LightScene::setup() {
    this->cube = new Cube;
    this->camera = new PerspectiveCamera(
        this->ctx->window,
        glm::vec3(-1.7f, -1.25f, 2.5f),
        25.9f,
        -35.0f,
        45.0f,
        .1,
        1000
    );

    glm::mat4 view = this->camera->deriveViewMetrix();
    glm::mat4 projection = this->camera->deriveProjectionMatrix();

    this->vertexShader = readShaderFile("./src/examples/17-light-scene/vertex-shader.vert");
    this->fragmentShader = readShaderFile("./src/examples/17-light-scene/fragment-shader.frag");
    this->lightFragmentShader = readShaderFile("./src/examples/17-light-scene/light-shader.frag");

    this->shaderProgram = new ShaderProgram;
    this->shaderProgram->attachShader(vertexShader);
    this->shaderProgram->attachShader(fragmentShader);
    this->shaderProgram->link();
    this->shaderProgram->use();
    this->shaderProgram->setUniformVec3("uObjectColor", glm::value_ptr(this->objectColor));
    this->shaderProgram->setUniformVec3("uLightColor", glm::value_ptr(this->lightColor));
    this->shaderProgram->setUniformVec3("uLightPosition", glm::value_ptr(this->lightPosition));
    this->shaderProgram->setUniformVec3("uViewPosition", glm::value_ptr(this->camera->position));
    this->shaderProgram->setUniformF("uAmbientStrength", _17_Config::ambient);
    this->shaderProgram->setUniformF("uSpecularStrength", _17_Config::specularStength);
    this->shaderProgram->setUniformI("uSpecularPower", _17_Config::specularPower);
    this->shaderProgram->setUniformMat4("uView", glm::value_ptr(view));
    this->shaderProgram->setUniformMat4("uProjection", glm::value_ptr(projection));

    this->lightShaderProgram = new ShaderProgram;
    this->lightShaderProgram->attachShader(vertexShader);
    this->lightShaderProgram->attachShader(lightFragmentShader);
    this->lightShaderProgram->link();
    this->lightShaderProgram->use();
    this->lightShaderProgram->setUniformVec3("uLightColor", glm::value_ptr(this->lightColor));
    this->lightShaderProgram->setUniformMat4("uView", glm::value_ptr(view));
    this->lightShaderProgram->setUniformMat4("uProjection", glm::value_ptr(projection));

    glUseProgram(0);

    glEnable(GL_DEPTH_TEST);
}

void _17_LightScene::render() {
    if (closeWindowOnEscPressed(ctx->window)) {
        this->setShouldExit(true);
        glfwSetWindowShouldClose(ctx->window, true);
        return;
    };

    if (glfwGetKey(this->ctx->window, GLFW_KEY_1) == GLFW_PRESS) {
        // Prevent from long press and capture single first Tab press
        if (!this->isPressingTab) {
            this->isPressingTab = true;
            this->isListeningMouseEvent = !this->isListeningMouseEvent;

            if (this->isListeningMouseEvent) {
                MouseMoveEvent::listen(this->ctx->window);
            } else {
                MouseMoveEvent::dismiss(this->ctx->window);
            }
        }
    } else {
        this->isPressingTab = false;
    }

    if (switchExampleOnArrowKeyPressed(ctx)) {
        this->setShouldExit(true);
        return;
    }
    
    glClearColor(.0f, .0f, .0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // When mouse is moving change yaw and pitch
    float newPitch = this->camera->getPitch() + MouseMoveEvent::offsetY;
    this->camera->setPitch(newPitch);
    this->camera->yaw += MouseMoveEvent::offsetX;

    this->camera->update(this->getDelta());

    glm::mat4 view = this->camera->deriveViewMetrix();
    glm::mat4 projection = this->camera->deriveProjectionMatrix();

    // Render object
    this->shaderProgram->use();
    this->shaderProgram->setUniformMat4("uProjection", glm::value_ptr(projection));
    this->shaderProgram->setUniformMat4("uView", glm::value_ptr(view));
    this->shaderProgram->setUniformF("uAmbientStrength", _17_Config::ambient);
    this->shaderProgram->setUniformF("uSpecularStrength", _17_Config::specularStength);
    this->shaderProgram->setUniformI("uSpecularPower", _17_Config::specularPower);
    this->shaderProgram->setUniformVec3("uViewPosition", glm::value_ptr(this->camera->position));
    this->cube
        ->setPosition(this->objectPosition)
        ->setScale(this->objectScale)
        ->render(this->shaderProgram);

    // Render light source
    this->lightShaderProgram->use();
    this->lightShaderProgram->setUniformMat4("uProjecton", glm::value_ptr(projection));
    this->lightShaderProgram->setUniformMat4("uView", glm::value_ptr(view));
    this->lightShaderProgram->setUniformVec3("uLightColor", glm::value_ptr(this->lightColor));
    this->cube
        ->setPosition(this->lightPosition)
        ->setScale(this->lightScale)
        ->render(this->lightShaderProgram);

    int index = ctx->gui->render(ctx->selectedExampleIndex, []() {
        ImGui::SetNextWindowPos(
            ImVec2(10.0f, 200.0f),
            ImGuiCond_Always
        );
        ImGui::SetNextWindowSizeConstraints(
            ImVec2(300, 0),        // min size
            ImVec2(300, 300)   // max size (height = 300 pixels)
        );

        ImGui::Begin("Phong Light Configuration");
        ImGui::SliderFloat("Ambient", &(_17_Config::ambient), 0.0f, 1.0f);
        ImGui::SliderFloat("Specular Strength", &(_17_Config::specularStength), 0.0f, 1.0f);
        ImGui::SliderInt("Specular Power", &(_17_Config::specularPower), 1, 128);

        ImGui::End();
    });
    if (index != ctx->selectedExampleIndex) {
        ctx->selectedExampleIndex = index;
        this->setShouldExit(true);
        return;
    }

    MouseMoveEvent::clearOffset();
    glfwSwapBuffers(ctx->window);
    glfwPollEvents();
}

void _17_LightScene::cleanup() {
    MouseMoveEvent::dismiss(this->ctx->window);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(lightFragmentShader);

    this->shaderProgram->dispose();
    this->lightShaderProgram->dispose();
    delete this->shaderProgram;
    delete this->lightShaderProgram;
    delete this->cube;
    delete this->camera;
}
