#include "main.h"
#include <iostream>
using namespace std;

namespace _22_MouseEvent {
    float lastX = .0f;
    float lastY = .0f;

    float offsetX = .0f;
    float offsetY = .0f;

    bool isCapturingEvent = false;
    bool isFirstEvent = false;
}

namespace _22_ScrollEvent {
    // Camera's field of view, useful for implementing zoom feature
    float fov = 45.0f;
}

void _22_onMouseMove(GLFWwindow* window, double xPos, double yPos) {
    if (_22_MouseEvent::isFirstEvent) {
        _22_MouseEvent::lastX = xPos;
        _22_MouseEvent::lastY = yPos;
        _22_MouseEvent::isFirstEvent = false;
        return;
    }

    // Offset Y is reversed as y-coord range from bottom to top
    float offsetX = xPos - _22_MouseEvent::lastX;
    float offsetY = (yPos - _22_MouseEvent::lastY) * -1;

    const float sensitivity = .1f;
    _22_MouseEvent::offsetX = offsetX * sensitivity;
    _22_MouseEvent::offsetY = offsetY * sensitivity;
    _22_MouseEvent::lastX = xPos;
    _22_MouseEvent::lastY = yPos;
};

void _22_onScroll(GLFWwindow* window, double xOffset, double yOffset) {
    _22_ScrollEvent::fov -= (float)yOffset;

    if (_22_ScrollEvent::fov < 1.0f) {
        _22_ScrollEvent::fov = 1.0f;
    } else if (_22_ScrollEvent::fov > 45.0f) {
        _22_ScrollEvent::fov = 45.0f;
    }
}

void _22_DirectionalLight::setup() {
    this->cube = new Cube;
    this->camera = new PerspectiveCamera(
        this->ctx->window,
        glm::vec3(.0f, .0f, 3.0f),
        .0f,
        -90.0f,
        _22_ScrollEvent::fov,
        .1,
        2000.0f
    );

    glm::mat4 view = this->camera->deriveViewMetrix();
    glm::mat4 projection = this->camera->deriveProjectionMatrix();

    this->vertexShader = readShaderFile("./src/examples/22-directional-light/vertex-shader.vert");
    this->fragmentShader = readShaderFile("./src/examples/22-directional-light/fragment-shader.frag");
    this->lightFragmentShader = readShaderFile("./src/examples/22-directional-light/light-shader.frag");

    // Read light map textures
    this->diffuseMap = new Texture2D(
        GL_TEXTURE0,
        "./assets/container2.png"
    );
    this->diffuseMap->load();

    this->specularMap = new Texture2D(
        GL_TEXTURE1,
        "./assets/container2_specular.png"
    );
    this->specularMap->load();

    this->shaderProgram = new ShaderProgram;
    this->shaderProgram->attachShader(vertexShader);
    this->shaderProgram->attachShader(fragmentShader);
    this->shaderProgram->link();
    this->shaderProgram->use();
    this->shaderProgram->setUniformVec3("uViewPosition", glm::value_ptr(this->camera->position));

    this->shaderProgram->setUniformVec3("uLight.position", glm::value_ptr(this->lightPosition));
    this->shaderProgram->setUniformVec3("uLight.ambient", glm::value_ptr(glm::vec3(.2f, .2f, .2f)));
    this->shaderProgram->setUniformVec3("uLight.diffuse", glm::value_ptr(glm::vec3(.5f, .5f, .5f)));
    this->shaderProgram->setUniformVec3("uLight.specular", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));

    // Setup diffuse map to texture unit 0, specular map to unit 1
    this->shaderProgram->setUniformI("uMaterial.diffuse", 0);
    this->shaderProgram->setUniformI("uMaterial.specular", 1);
    this->shaderProgram->setUniformF("uMaterial.shininess", 32.0f);

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

void _22_DirectionalLight::render() {
    if (closeWindowOnEscPressed(ctx->window)) {
        this->setShouldExit(true);
        glfwSetWindowShouldClose(ctx->window, true);
        return;
    };

    if (glfwGetKey(this->ctx->window, GLFW_KEY_1) == GLFW_PRESS) {
        // Prevent from long press and capture single first Tab press
        if (!this->isPressingTab) {
            this->isPressingTab = true;
            _22_MouseEvent::isCapturingEvent = !_22_MouseEvent::isCapturingEvent;
            glfwSetInputMode(
                this->ctx->window,
                GLFW_CURSOR,
                _22_MouseEvent::isCapturingEvent ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL
            );

            glfwSetCursorPosCallback(
                this->ctx->window,
                _22_MouseEvent::isCapturingEvent ? _22_onMouseMove : nullptr
            );

            glfwSetScrollCallback(
                this->ctx->window,
                _22_MouseEvent::isCapturingEvent ? _22_onScroll : nullptr
            );

            if (_22_MouseEvent::isCapturingEvent) {
                _22_MouseEvent::isFirstEvent = true;
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
    float newPitch = this->camera->getPitch() + _22_MouseEvent::offsetY;
    this->camera->setPitch(newPitch);
    this->camera->yaw += _22_MouseEvent::offsetX;

    // reset to prevent from using the current offset to move the pitch / yaw in next frame
    _22_MouseEvent::offsetX = .0f;
    _22_MouseEvent::offsetY = .0f;

    this->camera->update(this->getDelta());

    glm::mat4 view = this->camera->deriveViewMetrix();
    glm::mat4 projection = this->camera->deriveProjectionMatrix();

    // Render object
    this->diffuseMap->use();
    this->specularMap->use();
    this->shaderProgram->use();
    this->shaderProgram->setUniformMat4("uProjection", glm::value_ptr(projection));
    this->shaderProgram->setUniformMat4("uView", glm::value_ptr(view));
    this->shaderProgram->setUniformVec3("uViewPosition", glm::value_ptr(this->camera->position));

    for (int i = 0; i < 10; i++) {
        float angle = 20.0f * i;

        this->cube
            ->setPosition(this->cubePositions[i])
            ->setScale(this->objectScale)
            ->setRotation(angle + (float)glfwGetTime() * 10.0f, glm::vec3(1.0f, 0.3f, 0.5f))
            ->render(this->shaderProgram);
    }

    // Render light source
    this->lightShaderProgram->use();
    this->lightShaderProgram->setUniformMat4("uProjecton", glm::value_ptr(projection));
    this->lightShaderProgram->setUniformMat4("uView", glm::value_ptr(view));
    this->lightShaderProgram->setUniformVec3("uLightColor", glm::value_ptr(this->lightColor));
    this->cube
        ->setPosition(this->lightPosition)
        ->setScale(this->lightScale)
        ->render(this->lightShaderProgram);

    int index = ctx->gui->render(ctx->selectedExampleIndex);
    if (index != ctx->selectedExampleIndex) {
        ctx->selectedExampleIndex = index;
        this->setShouldExit(true);
        return;
    }

    glfwSwapBuffers(ctx->window);
    glfwPollEvents();
}

void _22_DirectionalLight::cleanup() {
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(lightFragmentShader);

    this->shaderProgram->dispose();
    this->lightShaderProgram->dispose();
    delete this->shaderProgram;
    delete this->lightShaderProgram;
    delete this->cube;
    delete this->diffuseMap;
    delete this->specularMap;
    delete this->camera;
}
