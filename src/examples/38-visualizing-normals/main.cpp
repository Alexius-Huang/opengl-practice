#include "main.h"

void _38_VisualizingNormals::setup() {
    this->camera = new PerspectiveCamera(
        this->ctx->window,
        glm::vec3(.0f, .0f, 3.0f),
        .0f,
        -90.0f,
        45.0f,
        .1,
        2000.0f
    );

    this->vertexShader = readShaderFile("./src/examples/38-visualizing-normals/shader.vert");
    this->fragmentShader = readShaderFile("./src/examples/38-visualizing-normals/shader.frag");

    glm::mat4 view = this->camera->deriveViewMetrix();
    glm::mat4 projection = this->camera->deriveProjectionMatrix();

    this->shaderProgram = new ShaderProgram;
    this->shaderProgram
        ->attachShader(vertexShader)
        ->attachShader(fragmentShader)
        ->link()
        ->use()
        ->setUniformMat4("uView", glm::value_ptr(view))
        ->setUniformMat4("uProjection", glm::value_ptr(projection));

    this->vizNormalVertexShader = readShaderFile("./src/examples/38-visualizing-normals/viz-normal.vert");
    this->vizNormalFragmentShader = readShaderFile("./src/examples/38-visualizing-normals/viz-normal.frag");
    this->vizNormalGeometryShader = readShaderFile("./src/examples/38-visualizing-normals/viz-normal.geom");
    this->vizNormalShaderProgram = new ShaderProgram;
    this->vizNormalShaderProgram
        ->attachShader(vizNormalVertexShader)
        ->attachShader(vizNormalFragmentShader)
        ->attachShader(vizNormalGeometryShader)
        ->link()
        ->use()
        ->setUniformMat4("uView", glm::value_ptr(view))
        ->setUniformMat4("uProjection", glm::value_ptr(projection));

    glUseProgram(0);

    this->model = new Model("./assets/backpack/backpack.obj");

    glEnable(GL_DEPTH_TEST);
}

void _38_VisualizingNormals::render() {
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
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down

    this->shaderProgram
        ->use()
        ->setUniformMat4("uProjection", glm::value_ptr(projection))
        ->setUniformMat4("uView", glm::value_ptr(view))
        ->setUniformMat4("uModel", glm::value_ptr(model));
    this->model->render(this->shaderProgram);

    this->vizNormalShaderProgram
        ->use()
        ->setUniformMat4("uProjection", glm::value_ptr(projection))
        ->setUniformMat4("uView", glm::value_ptr(view))
        ->setUniformMat4("uModel", glm::value_ptr(model));
    this->model->render(this->vizNormalShaderProgram);

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

void _38_VisualizingNormals::cleanup() {
    MouseMoveEvent::dismiss(this->ctx->window);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(vizNormalVertexShader);
    glDeleteShader(vizNormalFragmentShader);
    glDeleteShader(vizNormalGeometryShader);

    this->shaderProgram->dispose();
    this->vizNormalShaderProgram->dispose();
    delete this->shaderProgram;
    delete this->vizNormalShaderProgram;
    delete this->camera;
    delete this->model;
}
