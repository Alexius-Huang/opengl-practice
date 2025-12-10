#include "main.h"

string _08_title = "Passing Data Using Uniforms";

void _08_passDataUsingUniforms(Context* ctx) {
    unsigned int vertexShader = readShaderFile("./src/examples/08-pass-data-using-uniforms/vertex-shader.vert");
    unsigned int fragmentShader = readShaderFile("./src/examples/08-pass-data-using-uniforms/fragment-shader.frag");

    ShaderProgram shaderProgram;
    shaderProgram.attachShader(vertexShader);
    shaderProgram.attachShader(fragmentShader);
    shaderProgram.link();

    float vertices[] = {
        // Position          // Color
        -0.5f, -0.5f, 0.0f,  1.0f,  .0f,  .0f,
        0.5f,  -0.5f, 0.0f,   .0f, 1.0f,  .0f,
        0.0f,   0.5f, 0.0f,   .0f,  .0f, 1.0f
    };

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO); 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(ctx->window))
    {
        closeWindowOnEscPressed(ctx->window);
        togglePolygonModeOnKeyPressed(ctx->window, GLFW_KEY_TAB);
        if (switchExampleOnArrowKeyPressed(ctx)) break;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.use();

        // We can pass dynamic data into shader through uniform
        float time = glfwGetTime();
        shaderProgram.setUniformF("uTime", time);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        int index = ctx->gui->render(ctx->selectedExampleIndex);
        if (index != ctx->selectedExampleIndex) {
            ctx->selectedExampleIndex = index;
            break;
        }

        glfwSwapBuffers(ctx->window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    shaderProgram.dispose();
}
