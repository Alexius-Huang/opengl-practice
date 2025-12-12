#include "main.h"

string _05_title = "Using Multiple Shaders";

void _05_usingMultipleShaders(Context* ctx) {
    unsigned int vertexShader =
        readShaderFile("./src/examples/05-using-multiple-shaders/vertex-shader.vert");
    unsigned int fragmentShader1 =
        readShaderFile("./src/examples/05-using-multiple-shaders/fragment-shader-1.frag");
    unsigned int fragmentShader2 =
        readShaderFile("./src/examples/05-using-multiple-shaders/fragment-shader-2.frag");

    ShaderProgram shaderProgram1;
    shaderProgram1.attachShader(vertexShader);
    shaderProgram1.attachShader(fragmentShader1);
    shaderProgram1.link();

    ShaderProgram shaderProgram2;
    shaderProgram2.attachShader(vertexShader);
    shaderProgram2.attachShader(fragmentShader2);
    shaderProgram2.link();

    // Instead of providing 6 vertices to represent 2 triangles, we can reuse vertex using
    // element buffer object (EBO)
    float vertices[] = {
        0.5f,  0.5f,  0.0f,  // top right
        0.5f,  -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f, 0.5f,  0.0f   // top left
    };

    unsigned int indices[] = {0, 1, 3, 1, 2, 3};

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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(ctx->window)) {
        closeWindowOnEscPressed(ctx->window);
        togglePolygonModeOnKeyPressed(ctx->window, GLFW_KEY_TAB);
        if (switchExampleOnArrowKeyPressed(ctx)) break;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        shaderProgram1.use();
        glDrawElements(GL_TRIANGLES,
                       3,  // Draw 3 vertices
                       GL_UNSIGNED_INT,
                       0  // Offset in EBO
        );

        shaderProgram2.use();
        glDrawElements(GL_TRIANGLES,
                       3,  // Draw 3 vertices
                       GL_UNSIGNED_INT,
                       (void*)(3 * sizeof(unsigned int))  // Offset in EBO
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
    glDeleteBuffers(1, &EBO);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader1);
    glDeleteShader(fragmentShader2);
    shaderProgram1.dispose();
    shaderProgram2.dispose();
}

void _05_UsingMultipleShaders::setup() {
    this->vertexShader =
        readShaderFile("./src/examples/05-using-multiple-shaders/vertex-shader.vert");
    this->fragmentShader1 =
        readShaderFile("./src/examples/05-using-multiple-shaders/fragment-shader-1.frag");
    this->fragmentShader2 =
        readShaderFile("./src/examples/05-using-multiple-shaders/fragment-shader-2.frag");

    this->shaderProgram1 = new ShaderProgram;
    this->shaderProgram1->attachShader(vertexShader);
    this->shaderProgram1->attachShader(fragmentShader1);
    this->shaderProgram1->link();

    this->shaderProgram2 = new ShaderProgram;
    this->shaderProgram2->attachShader(vertexShader);
    this->shaderProgram2->attachShader(fragmentShader2);
    this->shaderProgram2->link();

    float vertices[] = {
        0.5f,  0.5f,  0.0f,  // top right
        0.5f,  -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f, 0.5f,  0.0f   // top left
    };

    unsigned int indices[] = {0, 1, 3, 1, 2, 3};

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void _05_UsingMultipleShaders::render() {
    closeWindowOnEscPressed(ctx->window);
    togglePolygonModeOnKeyPressed(ctx->window, GLFW_KEY_TAB);
    if (switchExampleOnArrowKeyPressed(ctx)) {
        this->setShouldExit(true);
        return;
    }

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    this->shaderProgram1->use();
    glDrawElements(GL_TRIANGLES,
                   3,  // Draw 3 vertices
                   GL_UNSIGNED_INT,
                   0  // Offset in EBO
    );

    this->shaderProgram2->use();
    glDrawElements(GL_TRIANGLES,
                   3,  // Draw 3 vertices
                   GL_UNSIGNED_INT,
                   (void*)(3 * sizeof(unsigned int))  // Offset in EBO
    );

    int index = ctx->gui->render(ctx->selectedExampleIndex);
    if (index != ctx->selectedExampleIndex) {
        ctx->selectedExampleIndex = index;
        this->setShouldExit(true);
        return;
    }

    glfwSwapBuffers(ctx->window);
    glfwPollEvents();
}

void _05_UsingMultipleShaders::cleanup() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader1);
    glDeleteShader(fragmentShader2);
    this->shaderProgram1->dispose();
    this->shaderProgram2->dispose();
}
