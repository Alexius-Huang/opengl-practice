#include "main.h"
#include <iostream>
using namespace std;

namespace _27_MouseEvent {
    float lastX = .0f;
    float lastY = .0f;

    float offsetX = .0f;
    float offsetY = .0f;

    bool isCapturingEvent = false;
    bool isFirstEvent = false;
}

namespace _27_ScrollEvent {
    // Camera's field of view, useful for implementing zoom feature
    float fov = 45.0f;
}

void _27_onMouseMove(GLFWwindow* window, double xPos, double yPos) {
    if (_27_MouseEvent::isFirstEvent) {
        _27_MouseEvent::lastX = xPos;
        _27_MouseEvent::lastY = yPos;
        _27_MouseEvent::isFirstEvent = false;
        return;
    }

    // Offset Y is reversed as y-coord range from bottom to top
    float offsetX = xPos - _27_MouseEvent::lastX;
    float offsetY = (yPos - _27_MouseEvent::lastY) * -1;

    const float sensitivity = .1f;
    _27_MouseEvent::offsetX = offsetX * sensitivity;
    _27_MouseEvent::offsetY = offsetY * sensitivity;
    _27_MouseEvent::lastX = xPos;
    _27_MouseEvent::lastY = yPos;
};

void _27_onScroll(GLFWwindow* window, double xOffset, double yOffset) {
    _27_ScrollEvent::fov -= (float)yOffset;

    if (_27_ScrollEvent::fov < 1.0f) {
        _27_ScrollEvent::fov = 1.0f;
    } else if (_27_ScrollEvent::fov > 45.0f) {
        _27_ScrollEvent::fov = 45.0f;
    }
}

void _27_StencilTest::setup() {
    this->cube = new Cube;
    this->floor = new Plane(5.0f);
    this->camera = new PerspectiveCamera(
        this->ctx->window,
        glm::vec3(0, 0, 3),
        0.0f,
        -90.0f,
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

    this->vertexShader = readShaderFile("./src/examples/27-stencil-test/vertex-shader.vert");
    this->fragmentShader = readShaderFile("./src/examples/27-stencil-test/fragment-shader.frag");
    this->stencilFragmentShader = readShaderFile("./src/examples/27-stencil-test/stencil-fragment-shader.frag");

    this->shaderProgram = new ShaderProgram;
    this->shaderProgram->attachShader(vertexShader);
    this->shaderProgram->attachShader(fragmentShader);
    this->shaderProgram->link();

    this->shaderProgram->use();
    this->shaderProgram->setUniformI("uTexture", 0);
    this->shaderProgram->setUniformMat4("uView", glm::value_ptr(this->camera->deriveViewMetrix()));
    this->shaderProgram->setUniformMat4("uProjection", glm::value_ptr(this->camera->deriveProjectionMatrix()));

    // Stencil shader program
    this->stencilShaderProgram = new ShaderProgram;
    this->stencilShaderProgram->attachShader(vertexShader);
    this->stencilShaderProgram->attachShader(stencilFragmentShader);
    this->stencilShaderProgram->link();

    this->stencilShaderProgram->use();
    this->stencilShaderProgram->setUniformMat4("uView", glm::value_ptr(this->camera->deriveViewMetrix()));
    this->stencilShaderProgram->setUniformMat4("uProjection", glm::value_ptr(this->camera->deriveProjectionMatrix()));

    glUseProgram(0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Enable stencil testing
    glEnable(GL_STENCIL_TEST);

    // When stencil or depth test failed, we preserve the value
    // However, when both test success, we want to write the value
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}

void _27_StencilTest::render() {
    if (closeWindowOnEscPressed(ctx->window)) {
        this->setShouldExit(true);
        glfwSetWindowShouldClose(ctx->window, true);
        return;
    };

    if (glfwGetKey(this->ctx->window, GLFW_KEY_TAB) == GLFW_PRESS) {
        // Prevent from long press and capture single first Tab press
        if (!this->isPressingTab) {
            this->isPressingTab = true;
            _27_MouseEvent::isCapturingEvent = !_27_MouseEvent::isCapturingEvent;
            glfwSetInputMode(
                this->ctx->window,
                GLFW_CURSOR,
                _27_MouseEvent::isCapturingEvent ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL
            );

            glfwSetCursorPosCallback(
                this->ctx->window,
                _27_MouseEvent::isCapturingEvent ? _27_onMouseMove : nullptr
            );

            glfwSetScrollCallback(
                this->ctx->window,
                _27_MouseEvent::isCapturingEvent ? _27_onScroll : nullptr
            );

            if (_27_MouseEvent::isCapturingEvent) {
                _27_MouseEvent::isFirstEvent = true;
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

    // We also need to clear Stencil buffer on enabling stencil test
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    this->textureCube->use();
    this->textureFloor->use();

    float newPitch = this->camera->getPitch() + _27_MouseEvent::offsetY;
    this->camera->setPitch(newPitch);
    this->camera->yaw += _27_MouseEvent::offsetX;
    _27_MouseEvent::offsetX = .0f;
    _27_MouseEvent::offsetY = .0f;

    this->camera->update(this->getDelta());

    this->shaderProgram->use();
    this->shaderProgram->setUniformMat4("uView", glm::value_ptr(this->camera->deriveViewMetrix()));
    this->shaderProgram->setUniformMat4("uProjection", glm::value_ptr(this->camera->deriveProjectionMatrix()));

    // Render floor, we do not want to write to stencil buffer
    glStencilMask(0x00);
    this->shaderProgram->setUniformI("uTexture", 1);
    this->floor->setPosition(glm::vec3(.0f, -.5f, .0f))
        ->setRotation(.0f, glm::vec3(1.0f, .0f, .0f))
        ->render(this->shaderProgram);

    // Render cubes, first if any fragment drawn, it will pass to the stencil buffer
    glStencilFunc(GL_ALWAYS, 1, 0xFF);

    // enable writing to stencil buffer
    glStencilMask(0xFF);

    // draw normal cubes and if any fragment drawn, it populates to the stencil buffer
    this->shaderProgram->setUniformI("uTexture", 0);
    this->cube->setPosition(glm::vec3(-1.0f, 0.0f, -1.0f))
        ->setScale(glm::vec3(1.0f, 1.0f, 1.0f))
        ->setRotation(.0f, glm::vec3(1.0f, .0f, .0f))
        ->render(this->shaderProgram);
    this->cube->setPosition(glm::vec3(2.0f, .0f, .0f))
        ->setScale(glm::vec3(1.0f, 1.0f, 1.0f))
        ->setRotation(.0f, glm::vec3(1.0f, .0f, .0f))
        ->render(this->shaderProgram);

    // draw the upscaled cube as stencil buffer outline
    // it will draw on the part where the stencil buffer bit is
    // not equal to 1, which means the outline of the cube
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);

    // disable writing to stencil buffer again
    glStencilMask(0x00);

    // disable the depth test to prevent from floor overriding
    glDisable(GL_DEPTH_TEST);

    // draw two scaled cubes with stencil shader program
    this->stencilShaderProgram->use();
    this->stencilShaderProgram->setUniformMat4("uView", glm::value_ptr(this->camera->deriveViewMetrix()));
    this->stencilShaderProgram->setUniformMat4("uProjection", glm::value_ptr(this->camera->deriveProjectionMatrix()));
    this->cube->setPosition(glm::vec3(-1.0f, 0.0f, -1.0f))
        ->setScale(glm::vec3(1.1f, 1.1f, 1.1f))
        ->setRotation(.0f, glm::vec3(1.0f, .0f, .0f))
        ->render(this->stencilShaderProgram);
    this->cube->setPosition(glm::vec3(2.0f, .0f, .0f))
        ->setScale(glm::vec3(1.1f, 1.1f, 1.1f))
        ->setRotation(.0f, glm::vec3(1.0f, .0f, .0f))
        ->render(this->stencilShaderProgram);

    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glEnable(GL_DEPTH_TEST);

    int index = ctx->gui->render(ctx->selectedExampleIndex);
    if (index != ctx->selectedExampleIndex) {
        ctx->selectedExampleIndex = index;
        this->setShouldExit(true);
        return;
    }

    glfwSwapBuffers(ctx->window);
    glfwPollEvents();
}

void _27_StencilTest::cleanup() {
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(stencilFragmentShader);

    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    this->shaderProgram->dispose();
    this->stencilShaderProgram->dispose();
    delete this->shaderProgram;
    delete this->stencilShaderProgram;
    delete this->textureCube;
    delete this->textureFloor;
    delete this->cube;
    delete this->floor;
}

