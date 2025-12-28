#include "main.h"
#include <iostream>
using namespace std;

namespace _28_MouseEvent {
    float lastX = .0f;
    float lastY = .0f;

    float offsetX = .0f;
    float offsetY = .0f;

    bool isCapturingEvent = false;
    bool isFirstEvent = false;
}

namespace _28_ScrollEvent {
    // Camera's field of view, useful for implementing zoom feature
    float fov = 45.0f;
}

void _28_onMouseMove(GLFWwindow* window, double xPos, double yPos) {
    if (_28_MouseEvent::isFirstEvent) {
        _28_MouseEvent::lastX = xPos;
        _28_MouseEvent::lastY = yPos;
        _28_MouseEvent::isFirstEvent = false;
        return;
    }

    // Offset Y is reversed as y-coord range from bottom to top
    float offsetX = xPos - _28_MouseEvent::lastX;
    float offsetY = (yPos - _28_MouseEvent::lastY) * -1;

    const float sensitivity = .1f;
    _28_MouseEvent::offsetX = offsetX * sensitivity;
    _28_MouseEvent::offsetY = offsetY * sensitivity;
    _28_MouseEvent::lastX = xPos;
    _28_MouseEvent::lastY = yPos;
};

void _28_onScroll(GLFWwindow* window, double xOffset, double yOffset) {
    _28_ScrollEvent::fov -= (float)yOffset;

    if (_28_ScrollEvent::fov < 1.0f) {
        _28_ScrollEvent::fov = 1.0f;
    } else if (_28_ScrollEvent::fov > 45.0f) {
        _28_ScrollEvent::fov = 45.0f;
    }
}

void _28_DiscardingFragment::setup() {
    this->cube = new Cube;
    this->floor = new Plane(10.0f);
    
    // We will render grass on plane with alpha channel enabled
    this->grass = new Plane;
    
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

    this->textureGrass = new Texture2D(
        GL_TEXTURE2,
        "./assets/grass.png",
        GL_RGBA
    );
    // Avoid edge interpolation when using alpha channel of the texture on border
    this->textureGrass->setTextureWrap(
        GL_CLAMP_TO_EDGE,
        GL_CLAMP_TO_EDGE
    );
    this->textureGrass->load();

    this->vertexShader = readShaderFile("./src/examples/28-discarding-fragment/vertex-shader.vert");
    this->fragmentShader = readShaderFile("./src/examples/28-discarding-fragment/fragment-shader.frag");

    this->shaderProgram = new ShaderProgram;
    this->shaderProgram->attachShader(vertexShader);
    this->shaderProgram->attachShader(fragmentShader);
    this->shaderProgram->link();

    // Setup texture to uniform, but activate shader first!
    this->shaderProgram->use();
    this->shaderProgram->setUniformI("uTexture", 0);
 
    // Pass in required matrices to shader
    this->shaderProgram->setUniformMat4("uView", glm::value_ptr(this->camera->deriveViewMetrix()));
    this->shaderProgram->setUniformMat4("uProjection", glm::value_ptr(this->camera->deriveProjectionMatrix()));

    glEnable(GL_DEPTH_TEST);

    // Use depth-function to adjust depth test
    glDepthFunc(GL_LESS);
}

void _28_DiscardingFragment::render() {
    if (closeWindowOnEscPressed(ctx->window)) {
        this->setShouldExit(true);
        glfwSetWindowShouldClose(ctx->window, true);
        return;
    };

    if (glfwGetKey(this->ctx->window, GLFW_KEY_TAB) == GLFW_PRESS) {
        // Prevent from long press and capture single first Tab press
        if (!this->isPressingTab) {
            this->isPressingTab = true;
            _28_MouseEvent::isCapturingEvent = !_28_MouseEvent::isCapturingEvent;
            glfwSetInputMode(
                this->ctx->window,
                GLFW_CURSOR,
                _28_MouseEvent::isCapturingEvent ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL
            );

            glfwSetCursorPosCallback(
                this->ctx->window,
                _28_MouseEvent::isCapturingEvent ? _28_onMouseMove : nullptr
            );

            glfwSetScrollCallback(
                this->ctx->window,
                _28_MouseEvent::isCapturingEvent ? _28_onScroll : nullptr
            );

            if (_28_MouseEvent::isCapturingEvent) {
                _28_MouseEvent::isFirstEvent = true;
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

    float newPitch = this->camera->getPitch() + _28_MouseEvent::offsetY;
    this->camera->setPitch(newPitch);
    this->camera->yaw += _28_MouseEvent::offsetX;
    _28_MouseEvent::offsetX = .0f;
    _28_MouseEvent::offsetY = .0f;

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

    // Render grasses:
    this->shaderProgram->setUniformI("uTexture", 2);
    for (const auto grassPosition : this->grassPositions) {
        this->grass->setPosition(grassPosition)
            ->setRotation(90.0f, glm::vec3(.0f, 1.0f, .0f))
            ->setRotation(90.0f, glm::vec3(1.0f, .0f, .0f))
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

void _28_DiscardingFragment::cleanup() {
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    this->shaderProgram->dispose();
    delete this->shaderProgram;
    delete this->textureCube;
    delete this->textureFloor;
    delete this->textureGrass;
    delete this->cube;
    delete this->floor;
    delete this->grass;
}

