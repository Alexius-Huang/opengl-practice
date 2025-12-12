#include "main.h"

void _01_HelloWorld::setup() {
    this->vertexShader = readShaderFile("./src/shaders/01-hello-world.vert");
    this->fragmentShader = readShaderFile("./src/shaders/01-hello-world.frag");

    shaderProgram = new ShaderProgram;
    shaderProgram->attachShader(vertexShader);
    shaderProgram->attachShader(fragmentShader);
    shaderProgram->link();

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO); 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);
}

void _01_HelloWorld::render() {
    // input
    // -----
    closeWindowOnEscPressed(ctx->window);
    togglePolygonModeOnKeyPressed(ctx->window, GLFW_KEY_TAB);
    if (switchExampleOnArrowKeyPressed(ctx)) {
        this->setShouldExit(true);
        return;
    };

    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the triangle
    shaderProgram->use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    int index = ctx->gui->render(ctx->selectedExampleIndex);
    if (index != ctx->selectedExampleIndex) {
        // Switching to other examples
        ctx->selectedExampleIndex = index;
        this->setShouldExit(true);
        return;
    }

    // glfw: swap buffers and poll IO events
    // -------------------------------------
    glfwSwapBuffers(ctx->window);
    glfwPollEvents();
}

void _01_HelloWorld::cleanup() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    shaderProgram->dispose();
}
