#include "main.h"
#include <iostream>
using namespace std;

namespace _17_MouseEvent {
    float lastX = .0f;
    float lastY = .0f;

    float offsetX = .0f;
    float offsetY = .0f;

    bool isCapturingEvent = false;
    bool isFirstEvent = false;
}

namespace _17_ScrollEvent {
    // Camera's field of view, useful for implementing zoom feature
    float fov = 45.0f;
}

namespace _17_Config {
    float ambient = .5f;
}

void _17_onMouseMove(GLFWwindow* window, double xPos, double yPos) {
    if (_17_MouseEvent::isFirstEvent) {
        _17_MouseEvent::lastX = xPos;
        _17_MouseEvent::lastY = yPos;
        _17_MouseEvent::isFirstEvent = false;
        return;
    }

    // Offset Y is reversed as y-coord range from bottom to top
    float offsetX = xPos - _17_MouseEvent::lastX;
    float offsetY = (yPos - _17_MouseEvent::lastY) * -1;

    const float sensitivity = .1f;
    _17_MouseEvent::offsetX = offsetX * sensitivity;
    _17_MouseEvent::offsetY = offsetY * sensitivity;
    _17_MouseEvent::lastX = xPos;
    _17_MouseEvent::lastY = yPos;
};

void _17_onScroll(GLFWwindow* window, double xOffset, double yOffset) {
    _17_ScrollEvent::fov -= (float)yOffset;

    if (_17_ScrollEvent::fov < 1.0f) {
        _17_ScrollEvent::fov = 1.0f;
    } else if (_17_ScrollEvent::fov > 45.0f) {
        _17_ScrollEvent::fov = 45.0f;
    }
}

void _17_LightScene::setup() {
    this->cube = new Cube;
    this->camera = new PerspectiveCamera(
        this->ctx->window,
        glm::vec3(1.5f, 3.0f, 5.0f),
        -30.0f,
        -110.0f,
        _17_ScrollEvent::fov,
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
    this->shaderProgram->setUniformF("uAmbientStrength", _17_Config::ambient);
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

    if (glfwGetKey(this->ctx->window, GLFW_KEY_TAB) == GLFW_PRESS) {
        // Prevent from long press and capture single first Tab press
        if (!this->isPressingTab) {
            this->isPressingTab = true;
            _17_MouseEvent::isCapturingEvent = !_17_MouseEvent::isCapturingEvent;
            glfwSetInputMode(
                this->ctx->window,
                GLFW_CURSOR,
                _17_MouseEvent::isCapturingEvent ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL
            );

            glfwSetCursorPosCallback(
                this->ctx->window,
                _17_MouseEvent::isCapturingEvent ? _17_onMouseMove : nullptr
            );

            glfwSetScrollCallback(
                this->ctx->window,
                _17_MouseEvent::isCapturingEvent ? _17_onScroll : nullptr
            );

            if (_17_MouseEvent::isCapturingEvent) {
                _17_MouseEvent::isFirstEvent = true;
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
    float newPitch = this->camera->getPitch() + _17_MouseEvent::offsetY;
    this->camera->setPitch(newPitch);
    this->camera->yaw += _17_MouseEvent::offsetX;

    // reset to prevent from using the current offset to move the pitch / yaw in next frame
    _17_MouseEvent::offsetX = .0f;
    _17_MouseEvent::offsetY = .0f;

    this->camera->update(this->getDelta());

    glm::mat4 view = this->camera->deriveViewMetrix();
    glm::mat4 projection = this->camera->deriveProjectionMatrix();

    // Render object
    this->shaderProgram->use();
    this->shaderProgram->setUniformMat4("uProjection", glm::value_ptr(projection));
    this->shaderProgram->setUniformMat4("uView", glm::value_ptr(view));
    this->shaderProgram->setUniformF("uAmbientStrength", _17_Config::ambient);
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
            ImVec2(FLT_MAX, 200)   // max size (height = 300 pixels)
        );

        ImGui::Begin("Phong Light Configuration");
        ImGui::SliderFloat("Ambient", &(_17_Config::ambient), 0.0f, 1.0f);
        ImGui::End();
    });
    if (index != ctx->selectedExampleIndex) {
        ctx->selectedExampleIndex = index;
        this->setShouldExit(true);
        return;
    }

    glfwSwapBuffers(ctx->window);
    glfwPollEvents();
}

void _17_LightScene::cleanup() {
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
