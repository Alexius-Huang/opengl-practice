#include "main.h"

void _34_CreateSkyboxUsingCubemap::setup() {
    this->cube = new Cube;
    this->camera = new PerspectiveCamera(
        this->ctx->window,
        glm::vec3(.0f, .0f, 3.0f),
        .0f,
        -90.0f,
        45.0f,
        .1,
        1000
    );

    glm::mat4 view = this->camera->deriveViewMetrix();
    glm::mat4 projection = this->camera->deriveProjectionMatrix();

    this->vertexShader = readShaderFile("./src/examples/34-create-skybox-using-cubemap/vertex-shader.vert");
    this->fragmentShader = readShaderFile("./src/examples/34-create-skybox-using-cubemap/fragment-shader.frag");

    this->shaderProgram = new ShaderProgram;
    this->shaderProgram
        ->attachShader(vertexShader)
        ->attachShader(fragmentShader)
        ->link()
        ->use()
        ->setUniformMat4("uView", glm::value_ptr(view))
        ->setUniformMat4("uProjection", glm::value_ptr(projection));

    // Create Skybox VAO, VBO
    glGenVertexArrays(1, &(this->skyboxVAO));
    glBindVertexArray(this->skyboxVAO);

    glGenBuffers(1, &(this->skyboxVBO));
    glBindBuffer(GL_ARRAY_BUFFER, this->skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->SKYBOX_VERTICES), this->SKYBOX_VERTICES, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind buffers, rebind it before we draw skybox
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Create skybox cubemap texture
    glGenTextures(1, &(this->textureSkybox));
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureSkybox);

    // Load skybox texture
    int texWidth, texHeight, nrChannels;
    std::vector<std::string> faces {
        "right.jpg",
        "left.jpg",
        "top.jpg",
        "bottom.jpg",
        "front.jpg",
        "back.jpg"
    };
    std::string path {"./assets/skybox/"};

    for (size_t i = 0; i < faces.size(); i++) {
        std::string face = faces[i];
        std::string fullPath { path + face };

        unsigned char* data = stbi_load(
            fullPath.c_str(),
            &texWidth,
            &texHeight,
            &nrChannels,
            0
        );

        if (!data) {
            stbi_image_free(data);
            throw std::runtime_error("ERROR::TEXTURE_CUBEMAP::CANNOT_LOAD_TEXTURE: loading " + fullPath + " failed");
        }

        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0,
            GL_RGB,
            texWidth,
            texHeight,
            0,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            data
        );
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // Setup Skybox shader program
    this->skyboxVertexShader = readShaderFile("./src/examples/34-create-skybox-using-cubemap/skybox-vertex-shader.vert");
    this->skyboxFragmentShader = readShaderFile("./src/examples/34-create-skybox-using-cubemap/skybox-fragment-shader.frag");

    this->skyboxShaderProgram = new ShaderProgram;
    this->skyboxShaderProgram
        ->attachShader(this->skyboxVertexShader)
        ->attachShader(this->skyboxFragmentShader)
        ->link()
        ->use()
        ->setUniformMat4("uView", glm::value_ptr(view))
        ->setUniformMat4("uProjection", glm::value_ptr(projection));

    glUseProgram(0);
    glEnable(GL_DEPTH_TEST);
}

void _34_CreateSkyboxUsingCubemap::render() {
    if (closeWindowOnEscPressed(ctx->window)) {
        this->setShouldExit(true);
        glfwSetWindowShouldClose(ctx->window, true);
        return;
    };

    if (glfwGetKey(this->ctx->window, GLFW_KEY_1) == GLFW_PRESS) {
        // Prevent from long press and capture single first Tab press
        if (!this->isPressingTab) {
            this->isPressingTab = true;
            this->isListeningMouseEvent = !this->isListeningMouseEvent;

            if (this->isListeningMouseEvent) {
                MouseMoveEvent::listen(this->ctx->window);
            } else {
                MouseMoveEvent::dismiss(this->ctx->window);
            }
        }
    } else {
        this->isPressingTab = false;
    }

    if (switchExampleOnArrowKeyPressed(ctx)) {
        this->setShouldExit(true);
        return;
    }
    
    glClearColor(.0f, .0f, .0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // When mouse is moving change yaw and pitch
    float newPitch = this->camera->getPitch() + MouseMoveEvent::offsetY;
    this->camera->setPitch(newPitch);
    this->camera->yaw += MouseMoveEvent::offsetX;

    this->camera->update(this->getDelta());

    glm::mat4 view = this->camera->deriveViewMetrix();
    glm::mat4 projection = this->camera->deriveProjectionMatrix();

    // Draw Skybox
    glDepthMask(GL_FALSE);
    glm::mat4 skyboxView = glm::mat4(glm::mat3(view));
    this->skyboxShaderProgram->use()
        ->setUniformMat4("uProjection", glm::value_ptr(projection))
        ->setUniformMat4("uView", glm::value_ptr(skyboxView));

    glBindVertexArray(this->skyboxVAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureSkybox);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);

    // Render object
    this->shaderProgram->use()
        ->setUniformMat4("uProjection", glm::value_ptr(projection))
        ->setUniformMat4("uView", glm::value_ptr(view));
    this->cube
        ->setPosition(glm::vec3(.0f, .0f, .0f))
        ->render(this->shaderProgram);

    int index = ctx->gui->render(ctx->selectedExampleIndex);
    if (index != ctx->selectedExampleIndex) {
        ctx->selectedExampleIndex = index;
        this->setShouldExit(true);
        return;
    }

    MouseMoveEvent::clearOffset();
    glfwSwapBuffers(ctx->window);
    glfwPollEvents();
}

void _34_CreateSkyboxUsingCubemap::cleanup() {
    MouseMoveEvent::dismiss(this->ctx->window);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(skyboxVertexShader);
    glDeleteShader(skyboxFragmentShader);

    this->shaderProgram->dispose();
    this->skyboxShaderProgram->dispose();
    delete this->shaderProgram;
    delete this->skyboxShaderProgram;
    delete this->cube;
    delete this->camera;
}
