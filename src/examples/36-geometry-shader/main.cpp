#include "main.h"

void _36_GeometryShader::setup() {
    this->vertexShader = readShaderFile("./src/examples/36-geometry-shader/shader.vert");
    this->fragmentShader = readShaderFile("./src/examples/36-geometry-shader/shader.frag");
    this->geometryShader = readShaderFile("./src/examples/36-geometry-shader/shader.geom");

    shaderProgram = new ShaderProgram;
    shaderProgram->attachShader(vertexShader);
    shaderProgram->attachShader(fragmentShader);
    shaderProgram->attachShader(geometryShader);
    shaderProgram->link();

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO); 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->POINTS), POINTS, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);
}

void _36_GeometryShader::render() {
    // input
    // -----
    if (closeWindowOnEscPressed(ctx->window)) {
        this->setShouldExit(true);
        glfwSetWindowShouldClose(ctx->window, true);
        return;
    };
    togglePolygonModeOnKeyPressed(ctx->window, GLFW_KEY_TAB);
    if (switchExampleOnArrowKeyPressed(ctx)) {
        this->setShouldExit(true);
        return;
    };

    // render
    // ------
    glClearColor(.1f, .1f, .1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the triangle
    shaderProgram->use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, 4);

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

void _36_GeometryShader::cleanup() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(geometryShader);
    shaderProgram->dispose();
}
