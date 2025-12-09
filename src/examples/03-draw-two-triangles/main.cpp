#include "main.h"

void _03_drawTwoTriangles(Context* ctx) {
    unsigned int vertexShader = readShaderFile("./src/shaders/01-hello-world.vert");
    unsigned int fragmentShader = readShaderFile("./src/shaders/01-hello-world.frag");

    ShaderProgram shaderProgram;
    shaderProgram.attachShader(vertexShader);
    shaderProgram.attachShader(fragmentShader);
    shaderProgram.link();

    float vertices[] = {
        /* top-left */
        -.5f, .5f, .0f,
        -.5f, -.5f, .0f,
        .5f, .5f, .0f,

        /* bottom-right */
        .5f, -.5f, .0f,
        .5f, .25f, .0f,
        -.25f, -.5f, .0f
    };

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO); 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(ctx->window))
    {
        // input
        // -----
        closeWindowOnEscPressed(ctx->window);
        togglePolygonModeOnKeyPressed(ctx->window, GLFW_KEY_TAB);
        if (switchExampleOnArrowKeyPressed(ctx)) break;

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.use();
        glBindVertexArray(VAO);

        // We have 6 vertices to draw
        glDrawArrays(GL_TRIANGLES, 0, 6);

        int index = ctx->gui->render(ctx->selectedExampleIndex);
        if (index != ctx->selectedExampleIndex) {
            // Switching to other examples
            ctx->selectedExampleIndex = index;
            break;
        }

        // glfw: swap buffers and poll IO events
        // -------------------------------------
        glfwSwapBuffers(ctx->window);
        glfwPollEvents();
    }

    // Cleanup
    // -------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    shaderProgram.dispose();
}

