#include "main.h"

void _07_PassDataUsingVertexAttributes::setup() {
    this->vertexShader =
        readShaderFile("./src/examples/07-pass-data-using-vertex-attributes/vertex-shader.vert");
    this->fragmentShader =
        readShaderFile("./src/examples/07-pass-data-using-vertex-attributes/fragment-shader.frag");

    this->shaderProgram = new ShaderProgram;
    this->shaderProgram->attachShader(vertexShader);
    this->shaderProgram->attachShader(fragmentShader);
    this->shaderProgram->link();

    float vertices[] = {// Position          // Color
                        -0.5f, -0.5f, 0.0f, 1.0f, .0f,  .0f,  0.5f, -0.5f, 0.0f,
                        .0f,   1.0f,  .0f,  0.0f, 0.5f, 0.0f, .0f,  .0f,   1.0f};

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Selecting positions:
    // - location = 0
    // - every 3 data represent X, Y, Z
    // - 6 data strides
    // - offset = 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Selecting colors:
    // - location = 1
    // - every 3 data represents R, G, B
    // - 6 data strides
    // - offset = 3 float data size
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void _07_PassDataUsingVertexAttributes::render() {
    closeWindowOnEscPressed(ctx->window);
    togglePolygonModeOnKeyPressed(ctx->window, GLFW_KEY_TAB);
    if (switchExampleOnArrowKeyPressed(ctx)) {
        this->setShouldExit(true);
        return;
    }

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    this->shaderProgram->use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    int index = ctx->gui->render(ctx->selectedExampleIndex);
    if (index != ctx->selectedExampleIndex) {
        ctx->selectedExampleIndex = index;
        this->setShouldExit(true);
        return;
    }

    glfwSwapBuffers(ctx->window);
    glfwPollEvents();
}

void _07_PassDataUsingVertexAttributes::cleanup() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    this->shaderProgram->dispose();
}
