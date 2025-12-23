#include "main.h"
#include <iostream>
using namespace std;

namespace _25_MouseEvent {
    float lastX = .0f;
    float lastY = .0f;

    float offsetX = .0f;
    float offsetY = .0f;

    bool isCapturingEvent = false;
    bool isFirstEvent = false;
}

namespace _25_ScrollEvent {
    // Camera's field of view, useful for implementing zoom feature
    float fov = 45.0f;
}

void _25_onMouseMove(GLFWwindow* window, double xPos, double yPos) {
    if (_25_MouseEvent::isFirstEvent) {
        _25_MouseEvent::lastX = xPos;
        _25_MouseEvent::lastY = yPos;
        _25_MouseEvent::isFirstEvent = false;
        return;
    }

    // Offset Y is reversed as y-coord range from bottom to top
    float offsetX = xPos - _25_MouseEvent::lastX;
    float offsetY = (yPos - _25_MouseEvent::lastY) * -1;

    const float sensitivity = .1f;
    _25_MouseEvent::offsetX = offsetX * sensitivity;
    _25_MouseEvent::offsetY = offsetY * sensitivity;
    _25_MouseEvent::lastX = xPos;
    _25_MouseEvent::lastY = yPos;
};

void _25_onScroll(GLFWwindow* window, double xOffset, double yOffset) {
    _25_ScrollEvent::fov -= (float)yOffset;

    if (_25_ScrollEvent::fov < 1.0f) {
        _25_ScrollEvent::fov = 1.0f;
    } else if (_25_ScrollEvent::fov > 45.0f) {
        _25_ScrollEvent::fov = 45.0f;
    }
}

void _25_LoadingModelUsingAssimp::setup() {
    this->camera = new PerspectiveCamera(
        this->ctx->window,
        glm::vec3(.0f, .0f, 3.0f),
        .0f,
        -90.0f,
        _25_ScrollEvent::fov,
        .1,
        2000.0f
    );

    glm::mat4 view = this->camera->deriveViewMetrix();
    glm::mat4 projection = this->camera->deriveProjectionMatrix();

    this->vertexShader = readShaderFile("./src/examples/25-loading-model-using-assimp/vertex-shader.vert");
    this->fragmentShader = readShaderFile("./src/examples/25-loading-model-using-assimp/fragment-shader.frag");

    this->shaderProgram = new ShaderProgram;
    this->shaderProgram->attachShader(vertexShader);
    this->shaderProgram->attachShader(fragmentShader);
    this->shaderProgram->link();
    this->shaderProgram->use();
    // this->shaderProgram->setUniformVec3("uViewPosition", glm::value_ptr(this->camera->position));

    // Set spot light position and direction based on camera
    // this->shaderProgram->setUniformVec3("uLight.position", glm::value_ptr(this->camera->position));
    // this->shaderProgram->setUniformVec3("uLight.direction", glm::value_ptr(this->camera->deriveCameraFrontVector()));
    // this->shaderProgram->setUniformF("uLight.cutoff", glm::cos(glm::radians(12.5f)));
    // this->shaderProgram->setUniformF("uLight.outerCutoff", glm::cos(glm::radians(17.5f)));

    // this->shaderProgram->setUniformVec3("uLight.ambient", glm::value_ptr(glm::vec3(.2f, .2f, .2f)));
    // this->shaderProgram->setUniformVec3("uLight.diffuse", glm::value_ptr(glm::vec3(.5f, .5f, .5f)));
    // this->shaderProgram->setUniformVec3("uLight.specular", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));

    // Setup diffuse map to texture unit 0, specular map to unit 1
    // this->shaderProgram->setUniformI("uMaterial.diffuse", 0);
    // this->shaderProgram->setUniformI("uMaterial.specular", 1);
    // this->shaderProgram->setUniformF("uMaterial.shininess", 32.0f);

    this->shaderProgram->setUniformMat4("uView", glm::value_ptr(view));
    this->shaderProgram->setUniformMat4("uProjection", glm::value_ptr(projection));

    glUseProgram(0);

    this->model = new Model("./assets/backpack/backpack.obj");

    glEnable(GL_DEPTH_TEST);
}

void _25_LoadingModelUsingAssimp::render() {
    if (closeWindowOnEscPressed(ctx->window)) {
        this->setShouldExit(true);
        glfwSetWindowShouldClose(ctx->window, true);
        return;
    };

    if (glfwGetKey(this->ctx->window, GLFW_KEY_1) == GLFW_PRESS) {
        // Prevent from long press and capture single first Tab press
        if (!this->isPressingTab) {
            this->isPressingTab = true;
            _25_MouseEvent::isCapturingEvent = !_25_MouseEvent::isCapturingEvent;
            glfwSetInputMode(
                this->ctx->window,
                GLFW_CURSOR,
                _25_MouseEvent::isCapturingEvent ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL
            );

            glfwSetCursorPosCallback(
                this->ctx->window,
                _25_MouseEvent::isCapturingEvent ? _25_onMouseMove : nullptr
            );

            glfwSetScrollCallback(
                this->ctx->window,
                _25_MouseEvent::isCapturingEvent ? _25_onScroll : nullptr
            );

            if (_25_MouseEvent::isCapturingEvent) {
                _25_MouseEvent::isFirstEvent = true;
            }
        }
    } else {
        this->isPressingTab = false;
    }

    if (switchExampleOnArrowKeyPressed(ctx)) {
        this->setShouldExit(true);
        return;
    }
    
    glClearColor(.05f, .05f, .05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // When mouse is moving change yaw and pitch
    float newPitch = this->camera->getPitch() + _25_MouseEvent::offsetY;
    this->camera->setPitch(newPitch);
    this->camera->yaw += _25_MouseEvent::offsetX;

    // reset to prevent from using the current offset to move the pitch / yaw in next frame
    _25_MouseEvent::offsetX = .0f;
    _25_MouseEvent::offsetY = .0f;

    this->camera->update(this->getDelta());

    glm::mat4 view = this->camera->deriveViewMetrix();
    glm::mat4 projection = this->camera->deriveProjectionMatrix();
    this->shaderProgram->setUniformMat4("uProjection", glm::value_ptr(projection));
    this->shaderProgram->setUniformMat4("uView", glm::value_ptr(view));

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
    this->shaderProgram->setUniformMat4("uModel", glm::value_ptr(model));
    this->model->render(this->shaderProgram);

    int index = ctx->gui->render(ctx->selectedExampleIndex);
    if (index != ctx->selectedExampleIndex) {
        ctx->selectedExampleIndex = index;
        this->setShouldExit(true);
        return;
    }

    glfwSwapBuffers(ctx->window);
    glfwPollEvents();
}

void _25_LoadingModelUsingAssimp::cleanup() {
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    this->shaderProgram->dispose();
    delete this->shaderProgram;
    delete this->camera;
    delete this->model;
}
