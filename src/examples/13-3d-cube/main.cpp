#include "main.h"

void _13_3DCube::setup() {
    // Initialize Cube
    this->cube = new Cube;
    this->cube->setRotation(-55.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    // Create view matrix to place the modal away from camera
    this->view = glm::mat4(1.0f);
    this->x = .0f;
    this->y = .0f;
    view = glm::translate(view, glm::vec3(this->x, this->y, -3.0f));

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

    this->vertexShader = readShaderFile("./src/examples/13-3d-cube/vertex-shader.vert");
    this->fragmentShader = readShaderFile("./src/examples/13-3d-cube/fragment-shader.frag");

    this->shaderProgram = new ShaderProgram;
    this->shaderProgram->attachShader(vertexShader);
    this->shaderProgram->attachShader(fragmentShader);
    this->shaderProgram->link();

    // Setup texture to uniform, but activate shader first!
    this->shaderProgram->use();
    this->shaderProgram->setUniformI("uTexture1", 0);
    this->shaderProgram->setUniformI("uTexture2", 1);

    // Pass in required matrices to shader
    this->shaderProgram->setUniformMat4("uView", glm::value_ptr(this->view));
    this->shaderProgram->setUniformMat4("uProjection", glm::value_ptr(this->projection));

    // Enable Depth Test!
    glEnable(GL_DEPTH_TEST);
}

void _13_3DCube::render() {
    if (closeWindowOnEscPressed(ctx->window)) {
        this->setShouldExit(true);
        glfwSetWindowShouldClose(ctx->window, true);
        return;
    };
    togglePolygonModeOnKeyPressed(ctx->window, GLFW_KEY_TAB);
    if (switchExampleOnArrowKeyPressed(ctx)) {
        this->setShouldExit(true);
        return;
    }

    this->translateOnWASDKeyPressed();
    
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // When depth bugger is enabled, we need to clear depth buffer bit as well
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    this->texture1->use();
    this->texture2->use();
    this->shaderProgram->use();

    this->generateTransformationMatrix();
    this->shaderProgram->setUniformMat4("uProjection", glm::value_ptr(this->projection));

    // Render the cube which already binds VAO / VBO
    // On the other hand, it internally creates model matrix and
    // pass it to the shader's `uModel` uniform
    this->cube->render(this->shaderProgram);

    int index = ctx->gui->render(ctx->selectedExampleIndex);
    if (index != ctx->selectedExampleIndex) {
        ctx->selectedExampleIndex = index;
        this->setShouldExit(true);
        return;
    }

    glfwSwapBuffers(ctx->window);
    glfwPollEvents();
}

void _13_3DCube::cleanup() {
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    this->shaderProgram->dispose();
    delete this->shaderProgram;
    delete this->texture1;
    delete this->texture2;
    delete this->cube;
}

// View might be resized, we need to generate projection matrix depend on aspect ratio
void _13_3DCube::generateTransformationMatrix() {
    float fov = glm::radians(45.0f);
    float near = .1f;
    float far = 100.0f;

    int w, h;
    glfwGetWindowSize(ctx->window, &w, &h);

    float aspectRatio = (float)w / (float)h;
    this->projection = glm::perspective(fov, aspectRatio, near, far);
}

void _13_3DCube::translateOnWASDKeyPressed() {
    float dt = this->getDelta();
    if (glfwGetKey(ctx->window, GLFW_KEY_D) == GLFW_PRESS) {
        this->x += dt;
    }
    
    if (glfwGetKey(ctx->window, GLFW_KEY_A) == GLFW_PRESS) {
        this->x -= dt;
    }

    if (glfwGetKey(ctx->window, GLFW_KEY_W) == GLFW_PRESS) {
        this->y += dt;
    }

    if (glfwGetKey(ctx->window, GLFW_KEY_S) == GLFW_PRESS) {
        this->y -= dt;
    }

    this->view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(this->x, this->y, -3.0f));
    this->shaderProgram->setUniformMat4("uView", glm::value_ptr(this->view));
}
