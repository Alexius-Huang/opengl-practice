#include "main.h"
#include <iostream>
using namespace std;

void _16_CameraControl::setup() {
    // Camera front is by default set to direction point to origin
    this->cameraFront = glm::normalize(glm::vec3(.0f, .0f, -.1f));
    this->cameraPosition = glm::vec3(.0f, .0f, 3.0f);

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

    this->vertexShader = readShaderFile("./src/examples/16-camera-control/vertex-shader.vert");
    this->fragmentShader = readShaderFile("./src/examples/16-camera-control/fragment-shader.frag");

    this->shaderProgram = new ShaderProgram;
    this->shaderProgram->attachShader(vertexShader);
    this->shaderProgram->attachShader(fragmentShader);
    this->shaderProgram->link();

    // Setup texture to uniform, but activate shader first!
    this->shaderProgram->use();
    this->shaderProgram->setUniformI("uTexture1", 0);
    this->shaderProgram->setUniformI("uTexture2", 1);

    // Pass in required matrices to shader
    // this->shaderProgram->setUniformMat4("uView", glm::value_ptr(this->view));
    this->shaderProgram->setUniformMat4("uProjection", glm::value_ptr(this->projection));

    float vertices[] = {
        // Vertices          // Texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Selecting positions:
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Selecting texture coordinates:
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Enable Depth Test!
    glEnable(GL_DEPTH_TEST);
}

void _16_CameraControl::render() {
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
    
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // When depth bugger is enabled, we need to clear depth buffer bit as well
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    this->texture1->use();
    this->texture2->use();
    this->shaderProgram->use();

    this->generateTransformationMatrix();
    this->shaderProgram->setUniformMat4("uProjection", glm::value_ptr(this->projection));

    // Create look at matrix which applies the transformation to the view matrix
    float radius { 10.0f };
    glm::vec3 cameraPosition = glm::vec3(
        sin(glfwGetTime()) * radius,
        0,
        cos(glfwGetTime()) * radius
    );

    // Check input and move camera
    float cameraSpeed = 2.5f;

    // Gram-Schmidt Process of deriving camera's coordinate
    glm::vec3 up = glm::vec3(.0f, 1.0f, .0f);
    glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, up));
    glm::vec3 cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));

    float delta = this->getDelta();
    if (glfwGetKey(ctx->window, GLFW_KEY_W) == GLFW_PRESS) {
        this->cameraPosition += this->cameraFront * cameraSpeed * delta;
    }
    if (glfwGetKey(ctx->window, GLFW_KEY_S) == GLFW_PRESS) {
        this->cameraPosition -= this->cameraFront * cameraSpeed * delta;
    }
    if (glfwGetKey(ctx->window, GLFW_KEY_D) == GLFW_PRESS) {
        this->cameraPosition += cameraRight * cameraSpeed * delta;
    }
    if (glfwGetKey(ctx->window, GLFW_KEY_A) == GLFW_PRESS) {
        this->cameraPosition -= cameraRight * cameraSpeed * delta;
    }

    // Derive look at transformation matrix
    glm::mat4 view = glm::lookAt(
        this->cameraPosition,
        this->cameraPosition + cameraFront,
        cameraUp
    );
    this->shaderProgram->setUniformMat4("uView", glm::value_ptr(view));

    // Create model matrix to place cubes and rotate it randomly
    glBindVertexArray(VAO);
    for (int i = 0; i < 10; i++) {
        glm::mat4 model = glm::mat4(1.0f);
        float angle = 20.0f * i;
        model = glm::translate(model, this->cubePositions[i]);
        model = glm::rotate(model, glm::radians(angle + (float)glfwGetTime() * 10), glm::vec3(1.0f, 0.3f, 0.5f));
        this->shaderProgram->setUniformMat4("uModel", glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    int index = ctx->gui->render(ctx->selectedExampleIndex);
    if (index != ctx->selectedExampleIndex) {
        ctx->selectedExampleIndex = index;
        this->setShouldExit(true);
        return;
    }

    glfwSwapBuffers(ctx->window);
    glfwPollEvents();
}

void _16_CameraControl::cleanup() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    this->shaderProgram->dispose();
    delete this->shaderProgram;
    delete this->texture1;
    delete this->texture2;
}

// View might be resized, we need to generate projection matrix depend on aspect ratio
void _16_CameraControl::generateTransformationMatrix() {
    float fov = glm::radians(45.0f);
    float near = .1f;
    float far = 100.0f;

    int w, h;
    glfwGetWindowSize(ctx->window, &w, &h);

    float aspectRatio = (float)w / (float)h;
    this->projection = glm::perspective(fov, aspectRatio, near, far);
}
