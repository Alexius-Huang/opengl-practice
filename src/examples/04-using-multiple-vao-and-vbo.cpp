#include "04-using-multiple-vao-and-vbo.h"

void _04_usingMultipleVAOAndVBO(Context* ctx) {
    unsigned int vertexShader = readShaderFile("./src/shaders/01-hello-world.vert");
    unsigned int fragmentShader = readShaderFile("./src/shaders/01-hello-world.frag");

    ShaderProgram shaderProgram;
    shaderProgram.attachShader(vertexShader);
    shaderProgram.attachShader(fragmentShader);
    shaderProgram.link();

    float vertices1[] = {
        /* top-left */
        -.5f, .5f, .0f,
        -.5f, -.5f, .0f,
        .5f, .5f, .0f,
    };

    float vertices2[] = {
        /* bottom-right */
        .5f, -.5f, .0f,
        .5f, .25f, .0f,
        -.25f, -.5f, .0f
    };

    // We can generate two VAOs at the same time
    unsigned int VAOs[2];
    glGenVertexArrays(2, VAOs);

    // This also applies to VBOs
    unsigned int VBOs[2];
    glGenBuffers(2, VBOs);

    // First triangle setup, bind to first vertex array then populate the vertex data in VBO
    glBindVertexArray(VAOs[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    // Second triangle setup, bind to second vertex array then populate the vertex data in VBO
    glBindVertexArray(VAOs[1]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    // Reset and unbind everything
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

        // Use first VAO
        shaderProgram.use();
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Switch to bind 2nd VAO
        shaderProgram.use();
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

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
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    shaderProgram.dispose();
}

