#include "main.h"
#include <iostream>
using namespace std;

namespace _29_MouseEvent {
    float lastX = .0f;
    float lastY = .0f;

    float offsetX = .0f;
    float offsetY = .0f;

    bool isCapturingEvent = false;
    bool isFirstEvent = false;
}

namespace _29_ScrollEvent {
    // Camera's field of view, useful for implementing zoom feature
    float fov = 45.0f;
}

void _29_onMouseMove(GLFWwindow* window, double xPos, double yPos) {
    if (_29_MouseEvent::isFirstEvent) {
        _29_MouseEvent::lastX = xPos;
        _29_MouseEvent::lastY = yPos;
        _29_MouseEvent::isFirstEvent = false;
        return;
    }

    // Offset Y is reversed as y-coord range from bottom to top
    float offsetX = xPos - _29_MouseEvent::lastX;
    float offsetY = (yPos - _29_MouseEvent::lastY) * -1;

    const float sensitivity = .1f;
    _29_MouseEvent::offsetX = offsetX * sensitivity;
    _29_MouseEvent::offsetY = offsetY * sensitivity;
    _29_MouseEvent::lastX = xPos;
    _29_MouseEvent::lastY = yPos;
};

void _29_onScroll(GLFWwindow* window, double xOffset, double yOffset) {
    _29_ScrollEvent::fov -= (float)yOffset;

    if (_29_ScrollEvent::fov < 1.0f) {
        _29_ScrollEvent::fov = 1.0f;
    } else if (_29_ScrollEvent::fov > 45.0f) {
        _29_ScrollEvent::fov = 45.0f;
    }
}

void _29_Blending::setup() {
    this->cube = new Cube;
    this->floor = new Plane(10.0f);
    
    // We will render grass on plane with alpha channel enabled
    this->window = new Plane;
    
    this->camera = new PerspectiveCamera(
        this->ctx->window,
        glm::vec3(-4.5f, 0.0f, 0.5f),
        0.0f,
        -30.0f,
        45,
        .1f,
        1000.0f
    );

    this->textureCube = new Texture2D(
        GL_TEXTURE0,
        "./assets/marble.jpg",
        GL_RGB
    );
    this->textureCube->load();

    this->textureFloor = new Texture2D(
        GL_TEXTURE1,
        "./assets/metal.png",
        GL_RGBA
    );
    this->textureFloor->load();

    this->textureWindow = new Texture2D(
        GL_TEXTURE2,
        "./assets/window.png",
        GL_RGBA
    );
    // Avoid edge interpolation when using alpha channel of the texture on border
    this->textureWindow->setTextureWrap(
        GL_CLAMP_TO_EDGE,
        GL_CLAMP_TO_EDGE
    );
    this->textureWindow->load();

    this->vertexShader = readShaderFile("./src/examples/29-blending/vertex-shader.vert");
    this->fragmentShader = readShaderFile("./src/examples/29-blending/fragment-shader.frag");

    this->shaderProgram = new ShaderProgram;
    this->shaderProgram->attachShader(vertexShader);
    this->shaderProgram->attachShader(fragmentShader);
    this->shaderProgram->link();

    this->shaderProgram->use();
    this->shaderProgram->setUniformI("uTexture", 0);
 
    this->shaderProgram->setUniformMat4("uView", glm::value_ptr(this->camera->deriveViewMetrix()));
    this->shaderProgram->setUniformMat4("uProjection", glm::value_ptr(this->camera->deriveProjectionMatrix()));

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Enable blend
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void _29_Blending::render() {
    if (closeWindowOnEscPressed(ctx->window)) {
        this->setShouldExit(true);
        glfwSetWindowShouldClose(ctx->window, true);
        return;
    };

    if (glfwGetKey(this->ctx->window, GLFW_KEY_TAB) == GLFW_PRESS) {
        // Prevent from long press and capture single first Tab press
        if (!this->isPressingTab) {
            this->isPressingTab = true;
            _29_MouseEvent::isCapturingEvent = !_29_MouseEvent::isCapturingEvent;
            glfwSetInputMode(
                this->ctx->window,
                GLFW_CURSOR,
                _29_MouseEvent::isCapturingEvent ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL
            );

            glfwSetCursorPosCallback(
                this->ctx->window,
                _29_MouseEvent::isCapturingEvent ? _29_onMouseMove : nullptr
            );

            glfwSetScrollCallback(
                this->ctx->window,
                _29_MouseEvent::isCapturingEvent ? _29_onScroll : nullptr
            );

            if (_29_MouseEvent::isCapturingEvent) {
                _29_MouseEvent::isFirstEvent = true;
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

    // When depth bugger is enabled, we need to clear depth buffer bit as well
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    this->textureCube->use();
    this->textureFloor->use();

    float newPitch = this->camera->getPitch() + _29_MouseEvent::offsetY;
    this->camera->setPitch(newPitch);
    this->camera->yaw += _29_MouseEvent::offsetX;
    _29_MouseEvent::offsetX = .0f;
    _29_MouseEvent::offsetY = .0f;

    this->camera->update(this->getDelta());

    this->shaderProgram->setUniformI("uTexture", 0);
    this->shaderProgram->setUniformMat4("uView", glm::value_ptr(this->camera->deriveViewMetrix()));
    this->shaderProgram->setUniformMat4("uProjection", glm::value_ptr(this->camera->deriveProjectionMatrix()));

    // Render cubes:
    this->cube->setPosition(glm::vec3(-1.0f, 0.0f, -1.0f))
        ->setRotation(.0f, glm::vec3(1.0f, .0f, .0f))
        ->render(this->shaderProgram);
    this->cube->setPosition(glm::vec3(2.0f, .0f, .0f))
        ->setRotation(.0f, glm::vec3(1.0f, .0f, .0f))
        ->render(this->shaderProgram);

    // Render floor:
    this->shaderProgram->setUniformI("uTexture", 1);
    this->floor->setPosition(glm::vec3(.0f, -.5f, .0f))
        ->setRotation(.0f, glm::vec3(1.0f, .0f, .0f))
        ->render(this->shaderProgram);

    // Render windows:
    // We must sort the windows from farthest to nearest w.r.t. camera because
    // of depth buffer when rendering window behind it will ignore transparency
    this->shaderProgram->setUniformI("uTexture", 2);

    std::map<float, glm::vec3> sorted;
    for (const auto windowPosition : this->windowPositions) {
        float distance = glm::length(this->camera->position - windowPosition);
        sorted[distance] = windowPosition;
    }

    for (
        std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin();
        it != sorted.rend();
        ++it
    ) {
        glm::vec3 windowPosition = it->second;
        this->window->setPosition(windowPosition)
            ->setRotation(90.0f, glm::vec3(.0f, .0f, 1.0f))
            ->render(this->shaderProgram);
    }

    int index = ctx->gui->render(ctx->selectedExampleIndex);
    if (index != ctx->selectedExampleIndex) {
        ctx->selectedExampleIndex = index;
        this->setShouldExit(true);
        return;
    }

    glfwSwapBuffers(ctx->window);
    glfwPollEvents();
}

void _29_Blending::cleanup() {
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDisable(GL_BLEND);

    this->shaderProgram->dispose();
    delete this->shaderProgram;
    delete this->textureCube;
    delete this->textureFloor;
    delete this->textureWindow;
    delete this->cube;
    delete this->floor;
    delete this->window;
}

