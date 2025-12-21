#include "main.h"
#include <iostream>
using namespace std;

namespace _21_MouseEvent {
    float lastX = .0f;
    float lastY = .0f;

    float offsetX = .0f;
    float offsetY = .0f;

    bool isCapturingEvent = false;
    bool isFirstEvent = false;
}

namespace _21_ScrollEvent {
    // Camera's field of view, useful for implementing zoom feature
    float fov = 45.0f;
}

void _21_onMouseMove(GLFWwindow* window, double xPos, double yPos) {
    if (_21_MouseEvent::isFirstEvent) {
        _21_MouseEvent::lastX = xPos;
        _21_MouseEvent::lastY = yPos;
        _21_MouseEvent::isFirstEvent = false;
        return;
    }

    // Offset Y is reversed as y-coord range from bottom to top
    float offsetX = xPos - _21_MouseEvent::lastX;
    float offsetY = (yPos - _21_MouseEvent::lastY) * -1;

    const float sensitivity = .1f;
    _21_MouseEvent::offsetX = offsetX * sensitivity;
    _21_MouseEvent::offsetY = offsetY * sensitivity;
    _21_MouseEvent::lastX = xPos;
    _21_MouseEvent::lastY = yPos;
};

void _21_onScroll(GLFWwindow* window, double xOffset, double yOffset) {
    _21_ScrollEvent::fov -= (float)yOffset;

    if (_21_ScrollEvent::fov < 1.0f) {
        _21_ScrollEvent::fov = 1.0f;
    } else if (_21_ScrollEvent::fov > 45.0f) {
        _21_ScrollEvent::fov = 45.0f;
    }
}

void _21_LightMaps::setup() {
    this->cube = new Cube;
    this->camera = new PerspectiveCamera(
        this->ctx->window,
        glm::vec3(-1.7f, -1.25f, 2.5f),
        25.9f,
        -35.0f,
        _21_ScrollEvent::fov,
        .1,
        2000.0f
    );

    glm::mat4 view = this->camera->deriveViewMetrix();
    glm::mat4 projection = this->camera->deriveProjectionMatrix();

    this->vertexShader = readShaderFile("./src/examples/21-light-maps/vertex-shader.vert");
    this->fragmentShader = readShaderFile("./src/examples/21-light-maps/fragment-shader.frag");
    this->lightFragmentShader = readShaderFile("./src/examples/21-light-maps/light-shader.frag");

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

    this->emissionMap = new Texture2D(
        GL_TEXTURE2,
        "./assets/matrix.jpg"
    );
    this->emissionMap->load();

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

    // Setup diffuse map to texture unit 0, specular map to unit 1 and emission map to 2
    this->shaderProgram->setUniformI("uMaterial.diffuse", 0);
    this->shaderProgram->setUniformI("uMaterial.specular", 1);
    this->shaderProgram->setUniformI("uMaterial.emission", 2);
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

void _21_LightMaps::render() {
    if (closeWindowOnEscPressed(ctx->window)) {
        this->setShouldExit(true);
        glfwSetWindowShouldClose(ctx->window, true);
        return;
    };

    if (glfwGetKey(this->ctx->window, GLFW_KEY_1) == GLFW_PRESS) {
        // Prevent from long press and capture single first Tab press
        if (!this->isPressingTab) {
            this->isPressingTab = true;
            _21_MouseEvent::isCapturingEvent = !_21_MouseEvent::isCapturingEvent;
            glfwSetInputMode(
                this->ctx->window,
                GLFW_CURSOR,
                _21_MouseEvent::isCapturingEvent ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL
            );

            glfwSetCursorPosCallback(
                this->ctx->window,
                _21_MouseEvent::isCapturingEvent ? _21_onMouseMove : nullptr
            );

            glfwSetScrollCallback(
                this->ctx->window,
                _21_MouseEvent::isCapturingEvent ? _21_onScroll : nullptr
            );

            if (_21_MouseEvent::isCapturingEvent) {
                _21_MouseEvent::isFirstEvent = true;
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
    float newPitch = this->camera->getPitch() + _21_MouseEvent::offsetY;
    this->camera->setPitch(newPitch);
    this->camera->yaw += _21_MouseEvent::offsetX;

    // reset to prevent from using the current offset to move the pitch / yaw in next frame
    _21_MouseEvent::offsetX = .0f;
    _21_MouseEvent::offsetY = .0f;

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

    int index = ctx->gui->render(ctx->selectedExampleIndex);
    if (index != ctx->selectedExampleIndex) {
        ctx->selectedExampleIndex = index;
        this->setShouldExit(true);
        return;
    }

    glfwSwapBuffers(ctx->window);
    glfwPollEvents();
}

void _21_LightMaps::cleanup() {
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
    delete this->emissionMap;
    delete this->camera;
}
