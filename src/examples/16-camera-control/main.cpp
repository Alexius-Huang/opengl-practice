#include "main.h"
#include <iostream>
using namespace std;

namespace _16_MouseEvent {
    float lastX = .0f;
    float lastY = .0f;

    float offsetX = .0f;
    float offsetY = .0f;

    bool isCapturingEvent = false;
    bool isFirstEvent = false;
}

namespace _16_ScrollEvent {
    // Camera's field of view, useful for implementing zoom feature
    float fov = 45.0f;
}

void onMouseMove(GLFWwindow* window, double xPos, double yPos) {
    if (_16_MouseEvent::isFirstEvent) {
        _16_MouseEvent::lastX = xPos;
        _16_MouseEvent::lastY = yPos;
        _16_MouseEvent::isFirstEvent = false;
        return;
    }

    // Offset Y is reversed as y-coord range from bottom to top
    float offsetX = xPos - _16_MouseEvent::lastX;
    float offsetY = (yPos - _16_MouseEvent::lastY) * -1;

    const float sensitivity = .1f;
    _16_MouseEvent::offsetX = offsetX * sensitivity;
    _16_MouseEvent::offsetY = offsetY * sensitivity;

    cout << "offset x: " << _16_MouseEvent::offsetX << " offset y: " << _16_MouseEvent::offsetY << endl;

    _16_MouseEvent::lastX = xPos;
    _16_MouseEvent::lastY = yPos;
};

void onScroll(GLFWwindow* window, double xOffset, double yOffset) {
    _16_ScrollEvent::fov -= (float)yOffset;

    if (_16_ScrollEvent::fov < 1.0f) {
        _16_ScrollEvent::fov = 1.0f;
    } else if (_16_ScrollEvent::fov > 45.0f) {
        _16_ScrollEvent::fov = 45.0f;
    }
}

void _16_CameraControl::setup() {
    this->cube = new Cube;

    // Create projection matrix for perspective projection
    this->generateTransformationMatrix();

    this->texture1 = new Texture2D(
        GL_TEXTURE0,
        "./assets/container.jpg",
        GL_RGB
    );
    this->texture1->load();

    this->texture2 = new Texture2D(
        GL_TEXTURE1,
        "./assets/awesome-face.png",
        GL_RGBA
    );
    this->texture2->load();

    this->vertexShader = readShaderFile("./src/examples/16-camera-control/vertex-shader.vert");
    this->fragmentShader = readShaderFile("./src/examples/16-camera-control/fragment-shader.frag");

    this->shaderProgram = new ShaderProgram;
    this->shaderProgram->attachShader(vertexShader);
    this->shaderProgram->attachShader(fragmentShader);
    this->shaderProgram->link();

    // Setup texture to uniform, but activate shader first!
    this->shaderProgram->use();
    this->shaderProgram->setUniformI("uTexture1", 0);
    this->shaderProgram->setUniformI("uTexture2", 1);

    // Pass in required matrices to shader
    // this->shaderProgram->setUniformMat4("uView", glm::value_ptr(this->view));
    this->shaderProgram->setUniformMat4("uProjection", glm::value_ptr(this->projection));

    glEnable(GL_DEPTH_TEST);
}

void _16_CameraControl::render() {
    if (closeWindowOnEscPressed(ctx->window)) {
        this->setShouldExit(true);
        glfwSetWindowShouldClose(ctx->window, true);
        return;
    };

    if (glfwGetKey(this->ctx->window, GLFW_KEY_TAB) == GLFW_PRESS) {
        // Prevent from long press and capture single first Tab press
        if (!this->isPressingTab) {
            this->isPressingTab = true;
            _16_MouseEvent::isCapturingEvent = !_16_MouseEvent::isCapturingEvent;
            glfwSetInputMode(
                this->ctx->window,
                GLFW_CURSOR,
                _16_MouseEvent::isCapturingEvent ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL
            );

            glfwSetCursorPosCallback(
                this->ctx->window,
                _16_MouseEvent::isCapturingEvent ? onMouseMove : nullptr
            );

            glfwSetScrollCallback(
                this->ctx->window,
                _16_MouseEvent::isCapturingEvent ? onScroll : nullptr
            );

            if (_16_MouseEvent::isCapturingEvent) {
                _16_MouseEvent::isFirstEvent = true;
            }
        }
    } else {
        this->isPressingTab = false;
    }

    if (switchExampleOnArrowKeyPressed(ctx)) {
        this->setShouldExit(true);
        return;
    }
    
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // When depth bugger is enabled, we need to clear depth buffer bit as well
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    this->texture1->use();
    this->texture2->use();

    this->generateTransformationMatrix();
    this->shaderProgram->setUniformMat4("uProjection", glm::value_ptr(this->projection));

    // Create look at matrix which applies the transformation to the view matrix
    float radius { 10.0f };
    glm::vec3 cameraPosition = glm::vec3(
        sin(glfwGetTime()) * radius,
        0,
        cos(glfwGetTime()) * radius
    );

    // Check input and move camera
    float cameraSpeed = 2.5f;

    // When mouse is moving change yaw and pitch
    this->yaw += _16_MouseEvent::offsetX;
    this->pitch += _16_MouseEvent::offsetY;

    // reset to prevent from using the current offset to move the pitch / yaw in next frame
    _16_MouseEvent::offsetX = .0f;
    _16_MouseEvent::offsetY = .0f;

    // Constraint pitch to not be able to pitch backward
    if (this->pitch > 89.0f) {
        this->pitch = 89.0f;
    } else if (this->pitch < -89.0f) {
        this->pitch = -89.0f;
    }

    // Gram-Schmidt Process of deriving camera's coordinate
    glm::vec3 cameraFront = this->deriveCameraFrontVector();
    glm::vec3 up = glm::vec3(.0f, 1.0f, .0f);
    glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, up));
    glm::vec3 cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));

    float delta = this->getDelta();
    if (glfwGetKey(ctx->window, GLFW_KEY_W) == GLFW_PRESS) {
        this->cameraPosition += cameraFront * cameraSpeed * delta;
    }
    if (glfwGetKey(ctx->window, GLFW_KEY_S) == GLFW_PRESS) {
        this->cameraPosition -= cameraFront * cameraSpeed * delta;
    }
    if (glfwGetKey(ctx->window, GLFW_KEY_D) == GLFW_PRESS) {
        this->cameraPosition += cameraRight * cameraSpeed * delta;
    }
    if (glfwGetKey(ctx->window, GLFW_KEY_A) == GLFW_PRESS) {
        this->cameraPosition -= cameraRight * cameraSpeed * delta;
    }

    // Derive look at transformation matrix
    glm::mat4 view = glm::lookAt(
        this->cameraPosition,
        this->cameraPosition + cameraFront,
        cameraUp
    );
    this->shaderProgram->setUniformMat4("uView", glm::value_ptr(view));

    // Renders cube which internally creates model matrix
    for (int i = 0; i < 10; i++) {
        float angle = 20.0f * i;
        this->cube->setPosition(this->cubePositions[i])
            ->setRotation(angle + (float)glfwGetTime() * 10, glm::vec3(1.0f, 0.3f, 0.5f))
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

void _16_CameraControl::cleanup() {
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    this->shaderProgram->dispose();
    delete this->shaderProgram;
    delete this->texture1;
    delete this->texture2;
    delete this->cube;
}

// View might be resized, we need to generate projection matrix depend on aspect ratio
void _16_CameraControl::generateTransformationMatrix() {
    float fov = glm::radians(_16_ScrollEvent::fov);
    float near = .1f;
    float far = 100.0f;

    int w, h;
    glfwGetWindowSize(ctx->window, &w, &h);

    float aspectRatio = (float)w / (float)h;
    this->projection = glm::perspective(fov, aspectRatio, near, far);
}
