#include "main.h"

namespace _26_ScrollEvent {
    // Camera's field of view, useful for implementing zoom feature
    float fov = 45.0f;
}

void _26_onScroll(GLFWwindow* window, double xOffset, double yOffset) {
    _26_ScrollEvent::fov -= (float)yOffset;

    if (_26_ScrollEvent::fov < 1.0f) {
        _26_ScrollEvent::fov = 1.0f;
    } else if (_26_ScrollEvent::fov > 45.0f) {
        _26_ScrollEvent::fov = 45.0f;
    }
}

void _26_DepthTest::setup() {
    this->cube = new Cube;
    this->floor = new Plane(10.0f);
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

    this->vertexShader = readShaderFile("./src/examples/26-depth-test/vertex-shader.vert");
    this->fragmentShader = readShaderFile("./src/examples/26-depth-test/fragment-shader.frag");

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

void _26_DepthTest::render() {
    if (closeWindowOnEscPressed(ctx->window)) {
        this->setShouldExit(true);
        glfwSetWindowShouldClose(ctx->window, true);
        return;
    };

    if (glfwGetKey(this->ctx->window, GLFW_KEY_TAB) == GLFW_PRESS) {
        // Prevent from long press and capture single first Tab press
        if (!this->isPressingTab) {
            this->isPressingTab = true;
            this->isListeningMouseEvent = !this->isListeningMouseEvent;

            if (this->isListeningMouseEvent) {
                MouseMoveEvent::listen(this->ctx->window);
            } else {
                MouseMoveEvent::dismiss(this->ctx->window);
            }

            glfwSetScrollCallback(
                this->ctx->window,
                this->isListeningMouseEvent ? _26_onScroll : nullptr
            );
        }
    } else {
        this->isPressingTab = false;
    }

    if (glfwGetKey(this->ctx->window, GLFW_KEY_1) == GLFW_PRESS) {
        this->depthMode = 1;
    } else if (glfwGetKey(this->ctx->window, GLFW_KEY_2) == GLFW_PRESS) {
        this->depthMode = 2;
    } else if (glfwGetKey(this->ctx->window, GLFW_KEY_3) == GLFW_PRESS) {
        this->depthMode = 3;
    } else if (glfwGetKey(this->ctx->window, GLFW_KEY_4) == GLFW_PRESS) {
        this->depthMode = 4;
    }
    this->shaderProgram->setUniformI("uMode", this->depthMode);

    if (switchExampleOnArrowKeyPressed(ctx)) {
        this->setShouldExit(true);
        return;
    }
    
    glClearColor(.0f, .0f, .0f, 1.0f);

    // When depth bugger is enabled, we need to clear depth buffer bit as well
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    this->textureCube->use();
    this->textureFloor->use();

    float newPitch = this->camera->getPitch() + MouseMoveEvent::offsetY;
    this->camera->setPitch(newPitch);
    this->camera->yaw += MouseMoveEvent::offsetX;

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

    int index = ctx->gui->render(ctx->selectedExampleIndex);
    if (index != ctx->selectedExampleIndex) {
        ctx->selectedExampleIndex = index;
        this->setShouldExit(true);
        return;
    }

    MouseMoveEvent::clearOffset();
    glfwSwapBuffers(ctx->window);
    glfwPollEvents();
}

void _26_DepthTest::cleanup() {
    MouseMoveEvent::dismiss(this->ctx->window);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    this->shaderProgram->dispose();
    delete this->shaderProgram;
    delete this->textureCube;
    delete this->textureFloor;
    delete this->cube;
    delete this->floor;
}

