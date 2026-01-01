#include "main.h"

void _40_InstancingThroughInstancedArrays::setup() {
    this->vertexShader = readShaderFile("./src/examples/40-instancing-through-instanced-arrays/shader.vert");
    this->fragmentShader = readShaderFile("./src/examples/40-instancing-through-instanced-arrays/shader.frag");

    this->shaderProgram = new ShaderProgram;
    this->shaderProgram
        ->attachShader(vertexShader)
        ->attachShader(fragmentShader)
        ->link()
        ->use()
        ->setUniformF("uTime", glfwGetTime());

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO); 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->VERTICES), this->VERTICES, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Calculate translation offsets for all instance
    float offset = .1f;
    int index = 0;
    glm::vec2 translationOffsets[100];
    for (int y = -10; y < 10; y += 2) {
        for (int x = -10; x < 10; x += 2, index++) {
            glm::vec2 translation;
            translation.x = (float)x / 10.0f + offset;
            translation.y = (float)y / 10.0f + offset;
            translationOffsets[index] = translation;
        }
    }

    // Create instanced array VBO
    glGenBuffers(1, &(this->instancedVBO));
    glBindBuffer(GL_ARRAY_BUFFER, this->instancedVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translationOffsets[0], GL_STATIC_DRAW);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(2, 1);

    glBindVertexArray(0);
}

void _40_InstancingThroughInstancedArrays::render() {
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
    shaderProgram
        ->use()
        ->setUniformF("uTime", glfwGetTime());
    glBindVertexArray(VAO);

    // instead of using glDrawArrays, we use glDrawArraysInstanced
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);

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

void _40_InstancingThroughInstancedArrays::cleanup() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &instancedVBO);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    shaderProgram->dispose();
}
