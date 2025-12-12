#include "main.h"

void _04_UsingMultipleVAOAndVBO::setup() {
    this->vertexShader = readShaderFile("./src/shaders/01-hello-world.vert");
    this->fragmentShader = readShaderFile("./src/shaders/01-hello-world.frag");

    this->shaderProgram = new ShaderProgram;
    this->shaderProgram->attachShader(vertexShader);
    this->shaderProgram->attachShader(fragmentShader);
    this->shaderProgram->link();

    float vertices1[] = {
        /* top-left */
        -.5f, .5f, .0f, -.5f, -.5f, .0f, .5f, .5f, .0f,
    };

    float vertices2[] = {/* bottom-right */
                         .5f, -.5f, .0f, .5f, .25f, .0f, -.25f, -.5f, .0f};

    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);

    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void _04_UsingMultipleVAOAndVBO::render() {
    closeWindowOnEscPressed(ctx->window);
    togglePolygonModeOnKeyPressed(ctx->window, GLFW_KEY_TAB);
    if (switchExampleOnArrowKeyPressed(ctx)) {
        this->setShouldExit(true);
        return;
    }

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    this->shaderProgram->use();

    glBindVertexArray(VAOs[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(VAOs[1]);
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

void _04_UsingMultipleVAOAndVBO::cleanup() {
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    this->shaderProgram->dispose();
}
