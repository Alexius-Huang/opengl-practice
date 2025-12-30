#include "main.h"

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

    this->textureCube = new Texture2D(GL_TEXTURE0, "./assets/marble.jpg");
    this->textureCube->load();

    this->textureFloor = new Texture2D(GL_TEXTURE1, "./assets/metal.png");
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

    MouseMoveEvent::clearOffset();
    glfwSwapBuffers(ctx->window);
    glfwPollEvents();
}

void _28_DiscardingFragment::cleanup() {
    MouseMoveEvent::dismiss(this->ctx->window);
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
