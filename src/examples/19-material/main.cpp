#include "main.h"

void _19_Material::setup() {
    this->cube = new Cube;
    this->camera = new PerspectiveCamera(
        this->ctx->window,
        glm::vec3(-1.7f, -1.25f, 2.5f),
        25.9f,
        -35.0f,
        45.0f,
        .1,
        2000.0f
    );

    glm::mat4 view = this->camera->deriveViewMetrix();
    glm::mat4 projection = this->camera->deriveProjectionMatrix();

    this->vertexShader = readShaderFile("./src/examples/19-material/vertex-shader.vert");
    this->fragmentShader = readShaderFile("./src/examples/19-material/fragment-shader.frag");
    this->lightFragmentShader = readShaderFile("./src/examples/19-material/light-shader.frag");

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

    this->shaderProgram->setUniformVec3("uMaterial.ambient", glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f)));
    this->shaderProgram->setUniformVec3("uMaterial.diffuse", glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f)));
    this->shaderProgram->setUniformVec3("uMaterial.specular", glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));
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

void _19_Material::render() {
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
    this->shaderProgram->use();
    this->shaderProgram->setUniformMat4("uProjection", glm::value_ptr(projection));
    this->shaderProgram->setUniformMat4("uView", glm::value_ptr(view));
    this->shaderProgram->setUniformVec3("uViewPosition", glm::value_ptr(this->camera->position));

    this->shaderProgram->setUniformVec3("uMaterial.ambient", glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f)));
    this->shaderProgram->setUniformVec3("uMaterial.diffuse", glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f)));
    this->shaderProgram->setUniformVec3("uMaterial.specular", glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));

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

    MouseMoveEvent::clearOffset();
    glfwSwapBuffers(ctx->window);
    glfwPollEvents();
}

void _19_Material::cleanup() {
    MouseMoveEvent::dismiss(this->ctx->window);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(lightFragmentShader);

    this->shaderProgram->dispose();
    this->lightShaderProgram->dispose();
    delete this->shaderProgram;
    delete this->lightShaderProgram;
    delete this->cube;
    delete this->camera;
}
