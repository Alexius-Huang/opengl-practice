#include "main.h"

namespace _23_ScrollEvent {
    // Camera's field of view, useful for implementing zoom feature
    float fov = 45.0f;
}

void _23_onScroll(GLFWwindow* window, double xOffset, double yOffset) {
    _23_ScrollEvent::fov -= (float)yOffset;

    if (_23_ScrollEvent::fov < 1.0f) {
        _23_ScrollEvent::fov = 1.0f;
    } else if (_23_ScrollEvent::fov > 45.0f) {
        _23_ScrollEvent::fov = 45.0f;
    }
}

void _23_PointLight::setup() {
    this->cube = new Cube;
    this->camera = new PerspectiveCamera(
        this->ctx->window,
        glm::vec3(.0f, .0f, 3.0f),
        .0f,
        -90.0f,
        _23_ScrollEvent::fov,
        .1,
        2000.0f
    );

    glm::mat4 view = this->camera->deriveViewMetrix();
    glm::mat4 projection = this->camera->deriveProjectionMatrix();

    this->vertexShader = readShaderFile("./src/examples/23-point-light/vertex-shader.vert");
    this->fragmentShader = readShaderFile("./src/examples/23-point-light/fragment-shader.frag");
    this->lightFragmentShader = readShaderFile("./src/examples/23-point-light/light-shader.frag");

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
    this->shaderProgram->setUniformVec3("uLight.attenuation", glm::value_ptr(this->lightAttenuation));
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

void _23_PointLight::render() {
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
                this->isListeningMouseEvent ? _23_onScroll : nullptr
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

    // moving light along z axis
    this->lightPosition.z = -4.0f + 6.0f * cos(glfwGetTime());

    // Render object
    this->diffuseMap->use();
    this->specularMap->use();
    this->shaderProgram->use();
    this->shaderProgram->setUniformMat4("uProjection", glm::value_ptr(projection));
    this->shaderProgram->setUniformMat4("uView", glm::value_ptr(view));
    this->shaderProgram->setUniformVec3("uViewPosition", glm::value_ptr(this->camera->position));
    this->shaderProgram->setUniformVec3("uLight.position", glm::value_ptr(this->lightPosition));

    for (int i = 0; i < 10; i++) {
        float angle = 20.0f * i;

        this->cube
            ->setPosition(this->cubePositions[i])
            ->setScale(this->objectScale)
            ->setRotation(angle + (float)glfwGetTime() * 10.0f, glm::vec3(1.0f, 0.3f, 0.5f))
            ->render(this->shaderProgram);
    }

    // Render point light source
    this->lightShaderProgram->use();
    this->lightShaderProgram->setUniformMat4("uProjecton", glm::value_ptr(projection));
    this->lightShaderProgram->setUniformMat4("uView", glm::value_ptr(view));
    this->lightShaderProgram->setUniformVec3("uLightColor", glm::value_ptr(this->lightColor));    
    this->cube
        ->setPosition(this->lightPosition)
        ->setScale(this->lightScale)
        ->setRotation(.0f, glm::vec3(1.0f, .0f, .0f))
        ->render(this->lightShaderProgram);

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

void _23_PointLight::cleanup() {
    MouseMoveEvent::dismiss(this->ctx->window);
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
