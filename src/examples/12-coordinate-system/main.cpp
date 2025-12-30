#include "main.h"

void _12_CoordinateSystem::setup() {
    // Create model matrix to place and rotate our model around x axis
    this->model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    // Create view matrix to place the modal away from camera
    this->view = glm::mat4(1.0f);
    this->x = .0f;
    this->y = .0f;
    view = glm::translate(view, glm::vec3(this->x, this->y, -3.0f));

    // Create projection matrix for perspective projection
    this->generateTransformationMatrix();

    this->texture1 = new Texture2D(GL_TEXTURE0, "./assets/container.jpg");
    this->texture1->load();

    this->texture2 = new Texture2D(GL_TEXTURE1, "./assets/awesome-face.png");
    this->texture2->load();

    this->vertexShader = readShaderFile("./src/examples/12-coordinate-system/vertex-shader.vert");
    this->fragmentShader = readShaderFile("./src/examples/12-coordinate-system/fragment-shader.frag");

    this->shaderProgram = new ShaderProgram;
    this->shaderProgram->attachShader(vertexShader);
    this->shaderProgram->attachShader(fragmentShader);
    this->shaderProgram->link();

    // Setup texture to uniform, but activate shader first!
    this->shaderProgram->use();
    this->shaderProgram->setUniformI("uTexture1", 0);
    this->shaderProgram->setUniformI("uTexture2", 1);

    // Pass in required matrices to shader
    this->shaderProgram->setUniformMat4("uModel", glm::value_ptr(this->model));
    this->shaderProgram->setUniformMat4("uView", glm::value_ptr(this->view));
    this->shaderProgram->setUniformMat4("uProjection", glm::value_ptr(this->projection));

    float vertices[] = {
        // positions        // colors         // texture coords
        0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top right
        0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom left
        -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
    };

    unsigned int indices[] = {
        0, 1, 3,  // first triangle
        1, 2, 3   // second triangle
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Selecting positions:
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Selecting colors:
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Selecting texture coordinates:
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void _12_CoordinateSystem::render() {
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
    glClear(GL_COLOR_BUFFER_BIT);

    this->texture1->use();
    this->texture2->use();
    this->shaderProgram->use();

    this->generateTransformationMatrix();
    this->shaderProgram->setUniformMat4("uProjection", glm::value_ptr(this->projection));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    int index = ctx->gui->render(ctx->selectedExampleIndex);
    if (index != ctx->selectedExampleIndex) {
        ctx->selectedExampleIndex = index;
        this->setShouldExit(true);
        return;
    }

    glfwSwapBuffers(ctx->window);
    glfwPollEvents();
}

void _12_CoordinateSystem::cleanup() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    this->shaderProgram->dispose();
    delete this->shaderProgram;
    delete this->texture1;
    delete this->texture2;
}

// View might be resized, we need to generate projection matrix depend on aspect ratio
void _12_CoordinateSystem::generateTransformationMatrix() {
    float fov = glm::radians(45.0f);
    float near = .1f;
    float far = 100.0f;

    int w, h;
    glfwGetWindowSize(ctx->window, &w, &h);

    float aspectRatio = (float)w / (float)h;
    this->projection = glm::perspective(fov, aspectRatio, near, far);
}

void _12_CoordinateSystem::translateOnWASDKeyPressed() {
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
