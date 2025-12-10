#include "main.h"

string _02_title = "Element Buffer Object";

void _02_elementBufferObject(Context* ctx) {
    unsigned int vertexShader = readShaderFile("./src/shaders/01-hello-world.vert");
    unsigned int fragmentShader = readShaderFile("./src/shaders/01-hello-world.frag");

    ShaderProgram shaderProgram;
    shaderProgram.attachShader(vertexShader);
    shaderProgram.attachShader(fragmentShader);
    shaderProgram.link();

    // Instead of providing 6 vertices to represent 2 triangles, we can reuse vertex using
    // element buffer object (EBO)
    float vertices[] = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Generate element buffer object and bind indices data
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    unsigned int VBO;
    glGenBuffers(1, &VBO); 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    // TODO: Clarify what are the following code means
    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 

    while (!glfwWindowShouldClose(ctx->window))
    {
        closeWindowOnEscPressed(ctx->window);
        togglePolygonModeOnKeyPressed(ctx->window, GLFW_KEY_TAB);
        if (switchExampleOnArrowKeyPressed(ctx)) break;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.use();
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(
            GL_TRIANGLES,
            6,                // Draw six vertices in total
            GL_UNSIGNED_INT,
            0                 // Offset in EBO
        );

        int index = ctx->gui->render(ctx->selectedExampleIndex);
        if (index != ctx->selectedExampleIndex) {
            // Switching to other examples
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
