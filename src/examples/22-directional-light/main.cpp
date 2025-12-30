#include "main.h"

namespace _22_ScrollEvent {
    // Camera's field of view, useful for implementing zoom feature
    float fov = 45.0f;
}

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
    this->shaderProgram->setUniformVec3("uLight.direction", glm::value_ptr(this->lightDirection));
    this->shaderProgram->setUniformVec3("uLight.ambient", glm::value_ptr(glm::vec3(.2f, .2f, .2f)));
    this->shaderProgram->setUniformVec3("uLight.diffuse", glm::value_ptr(glm::vec3(.5f, .5f, .5f)));
    this->shaderProgram->setUniformVec3("uLight.specular", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));

    // Setup diffuse map to texture unit 0, specular map to unit 1
    this->shaderProgram->setUniformI("uMaterial.diffuse", 0);
    this->shaderProgram->setUniformI("uMaterial.specular", 1);
    this->shaderProgram->setUniformF("uMaterial.shininess", 32.0f);

    this->shaderProgram->setUniformMat4("uView", glm::value_ptr(view));
    this->shaderProgram->setUniformMat4("uProjection", glm::value_ptr(projection));

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
            this->isListeningMouseEvent = !this->isListeningMouseEvent;

            if (this->isListeningMouseEvent) {
                MouseMoveEvent::listen(this->ctx->window);
            } else {
                MouseMoveEvent::dismiss(this->ctx->window);
            }

            glfwSetScrollCallback(
                this->ctx->window,
                this->isListeningMouseEvent ? _22_onScroll : nullptr
            );
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

void _22_DirectionalLight::cleanup() {
    MouseMoveEvent::dismiss(this->ctx->window);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    this->shaderProgram->dispose();
    delete this->shaderProgram;
    delete this->cube;
    delete this->diffuseMap;
    delete this->specularMap;
    delete this->camera;
}
