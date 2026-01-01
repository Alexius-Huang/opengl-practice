#include "main.h"

void _37_ExplosionEffectUsingGeometryShader::setup() {
    this->camera = new PerspectiveCamera(
        this->ctx->window,
        glm::vec3(.0f, .0f, 3.0f),
        .0f,
        -90.0f,
        45.0f,
        .1,
        2000.0f
    );

    this->vertexShader = readShaderFile("./src/examples/37-explosion-effect-using-geometry-shader/shader.vert");
    this->fragmentShader = readShaderFile("./src/examples/37-explosion-effect-using-geometry-shader/shader.frag");
    this->geometryShader = readShaderFile("./src/examples/37-explosion-effect-using-geometry-shader/shader.geom");

    glm::mat4 view = this->camera->deriveViewMetrix();
    glm::mat4 projection = this->camera->deriveProjectionMatrix();

    this->shaderProgram = new ShaderProgram;
    this->shaderProgram
        ->attachShader(vertexShader)
        ->attachShader(fragmentShader)
        ->attachShader(geometryShader)
        ->link()
        ->use()
        ->setUniformMat4("uView", glm::value_ptr(view))
        ->setUniformMat4("uProjection", glm::value_ptr(projection))
        ->setUniformF("uTime", glfwGetTime());

    glUseProgram(0);

    std::cout << "Setup complete!" << std::endl;

    this->model = new Model("./assets/backpack/backpack.obj");

    glEnable(GL_DEPTH_TEST);
}

void _37_ExplosionEffectUsingGeometryShader::render() {
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
    
    glClearColor(.05f, .05f, .05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // When mouse is moving change yaw and pitch
    float newPitch = this->camera->getPitch() + MouseMoveEvent::offsetY;
    this->camera->setPitch(newPitch);
    this->camera->yaw += MouseMoveEvent::offsetX;

    this->camera->update(this->getDelta());

    glm::mat4 view = this->camera->deriveViewMetrix();
    glm::mat4 projection = this->camera->deriveProjectionMatrix();
    this->shaderProgram
        ->setUniformMat4("uProjection", glm::value_ptr(projection))
        ->setUniformMat4("uView", glm::value_ptr(view))
        ->setUniformF("uTime", glfwGetTime());

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
    this->shaderProgram->setUniformMat4("uModel", glm::value_ptr(model));
    this->model->render(this->shaderProgram);

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

void _37_ExplosionEffectUsingGeometryShader::cleanup() {
    MouseMoveEvent::dismiss(this->ctx->window);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(geometryShader);

    this->shaderProgram->dispose();
    delete this->shaderProgram;
    delete this->camera;
    delete this->model;
}
