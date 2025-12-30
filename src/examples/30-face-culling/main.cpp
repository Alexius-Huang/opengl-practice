#include "main.h"

void _30_FaceCulling::setup() {
    this->cube = new Cube;
    
    this->camera = new PerspectiveCamera(
        this->ctx->window,
        glm::vec3(-2.5f, 0.0f, 1.0f),
        0.0f,
        -30.0f,
        45,
        .1f,
        1000.0f
    );

    this->textureCube = new Texture2D(GL_TEXTURE0, "./assets/marble.jpg");
    this->textureCube->load();

    this->vertexShader = readShaderFile("./src/examples/30-face-culling/vertex-shader.vert");
    this->fragmentShader = readShaderFile("./src/examples/30-face-culling/fragment-shader.frag");

    this->shaderProgram = new ShaderProgram;
    this->shaderProgram->attachShader(vertexShader);
    this->shaderProgram->attachShader(fragmentShader);
    this->shaderProgram->link();

    this->shaderProgram->use();
    this->shaderProgram->setUniformI("uTexture", 0);
 
    this->shaderProgram->setUniformMat4("uView", glm::value_ptr(this->camera->deriveViewMetrix()));
    this->shaderProgram->setUniformMat4("uProjection", glm::value_ptr(this->camera->deriveProjectionMatrix()));

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // enable face culling
    glEnable(GL_CULL_FACE);

    // by default cull the back face with front face as CCW
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

void _30_FaceCulling::render() {
    if (closeWindowOnEscPressed(ctx->window)) {
        this->setShouldExit(true);
        glfwSetWindowShouldClose(ctx->window, true);
        return;
    };

    if (glfwGetKey(this->ctx->window, GLFW_KEY_TAB) == GLFW_PRESS) {
        // Prevent from long press and capture single first Tab press
        if (!this->isPressingTab) {
            this->isPressingTab = true;
            this->isListeningMouseEvents = !this->isListeningMouseEvents;

            if (this->isListeningMouseEvents) {
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

    // 1: default, cull: back face; Front face is Counterclockwise (CCW)
    // 2: cull: front face; front face: Counterclockwise
    // 3: cull: back face; front face: Clockwise (CW)
    // 4: cull: front face; front face: Clockwise
    if (glfwGetKey(this->ctx->window, GLFW_KEY_1) == GLFW_PRESS) {
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
    } else if (glfwGetKey(this->ctx->window, GLFW_KEY_2) == GLFW_PRESS) {
        glCullFace(GL_FRONT);
        glFrontFace(GL_CCW);
    } else if (glfwGetKey(this->ctx->window, GLFW_KEY_3) == GLFW_PRESS) {
        glCullFace(GL_BACK);
        glFrontFace(GL_CW);
    } else if (glfwGetKey(this->ctx->window, GLFW_KEY_4) == GLFW_PRESS) {
        glCullFace(GL_FRONT);
        glFrontFace(GL_CW);
    }
    
    glClearColor(.0f, .0f, .0f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    this->textureCube->use();

    float newPitch = this->camera->getPitch() + MouseMoveEvent::offsetY;
    this->camera->setPitch(newPitch);
    this->camera->yaw += MouseMoveEvent::offsetX;

    this->camera->update(this->getDelta());

    this->shaderProgram->setUniformI("uTexture", 0);
    this->shaderProgram->setUniformMat4("uView", glm::value_ptr(this->camera->deriveViewMetrix()));
    this->shaderProgram->setUniformMat4("uProjection", glm::value_ptr(this->camera->deriveProjectionMatrix()));

    // Render cubes:
    this->cube->setPosition(glm::vec3(.0f, .0f, .0f))
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

void _30_FaceCulling::cleanup() {
    MouseMoveEvent::dismiss(this->ctx->window);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glDisable(GL_CULL_FACE);

    this->shaderProgram->dispose();
    delete this->shaderProgram;
    delete this->textureCube;
    delete this->cube;
}
