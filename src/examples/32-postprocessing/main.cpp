#include "main.h"

void _32_Postprocessing::setup() {
    this->cube = new Cube;
    this->floor = new Plane(10.0f);
    this->camera = new PerspectiveCamera(
        this->ctx->window,
        glm::vec3(0, 0, 3),
        0.0f,
        -90.0f,
        45,
        .1f,
        1000.0f
    );

    this->textureCube = new Texture2D(GL_TEXTURE0, "./assets/container.jpg");
    this->textureCube->load();

    this->textureFloor = new Texture2D(GL_TEXTURE1, "./assets/metal.png");
    this->textureFloor->load();

    this->vertexShader = readShaderFile("./src/examples/32-postprocessing/vertex-shader.vert");
    this->fragmentShader = readShaderFile("./src/examples/32-postprocessing/fragment-shader.frag");

    this->shaderProgram = new ShaderProgram;
    this->shaderProgram->attachShader(vertexShader);
    this->shaderProgram->attachShader(fragmentShader);
    this->shaderProgram->link();

    // Setup texture to uniform, but activate shader first!
    this->shaderProgram->use();
    this->shaderProgram->setUniformI("uTexture", 0);
 
    // Pass in required matrices to shader
    this->shaderProgram->setUniformMat4("uView", glm::value_ptr(this->camera->deriveViewMetrix()));
    this->shaderProgram->setUniformMat4("uProjection", glm::value_ptr(this->camera->deriveProjectionMatrix()));

    this->frameBufferVertexShader = readShaderFile("./src/examples/32-postprocessing/frame-buffer-vertex-shader.vert");
    this->frameBufferFragmentShader = readShaderFile("./src/examples/32-postprocessing/frame-buffer-fragment-shader.frag");

    this->frameBufferShaderProgram = new ShaderProgram;
    this->frameBufferShaderProgram->attachShader(frameBufferVertexShader);
    this->frameBufferShaderProgram->attachShader(frameBufferFragmentShader);
    this->frameBufferShaderProgram->link();

    this->frameBufferShaderProgram->use();
    this->frameBufferShaderProgram->setUniformI("uScreenTexture", 0);

    // Generate Frame buffer object
    glGenFramebuffers(1, &(this->FBO));
    glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);

    // Get correct framebuffer size
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(ctx->window, &fbWidth, &fbHeight);

    // Create color attachment texture
    glGenTextures(1, &(this->textureColorBuffer));
    glBindTexture(GL_TEXTURE_2D, this->textureColorBuffer);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fbWidth, fbHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->textureColorBuffer, 0);

    // Create Render buffer object
    glGenRenderbuffers(1, &(this->RBO));
    glBindRenderbuffer(GL_RENDERBUFFER, this->RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, fbWidth, fbHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->RBO); // now actually attach it

    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Generate Quad's VAO and VBO
    glGenVertexArrays(1, &(this->quadVAO));
    glGenBuffers(1, &(this->quadVBO));
    glBindVertexArray(this->quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(QUAD_VERTICES), &QUAD_VERTICES, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glUseProgram(0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

void _32_Postprocessing::render() {
    if (closeWindowOnEscPressed(ctx->window)) {
        this->setShouldExit(true);
        glfwSetWindowShouldClose(ctx->window, true);
        return;
    };

    if (glfwGetKey(this->ctx->window, GLFW_KEY_TAB) == GLFW_PRESS) {
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

    if (glfwGetKey(this->ctx->window, GLFW_KEY_1) == GLFW_PRESS) {
        this->uMode = 1;
    } else if (glfwGetKey(this->ctx->window, GLFW_KEY_2) == GLFW_PRESS) {
        this->uMode = 2;
    } else if (glfwGetKey(this->ctx->window, GLFW_KEY_3) == GLFW_PRESS) {
        this->uMode = 3;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
    glEnable(GL_DEPTH_TEST);

    // Clear framebuffer content
    glClearColor(.1f, .1f, .1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    this->textureCube->use();
    this->textureFloor->use();

    float newPitch = this->camera->getPitch() + MouseMoveEvent::offsetY;
    this->camera->setPitch(newPitch);
    this->camera->yaw += MouseMoveEvent::offsetX;

    this->camera->update(this->getDelta());

    this->shaderProgram->use();
    this->shaderProgram->setUniformI("uTexture", 0);
    this->shaderProgram->setUniformMat4("uView", glm::value_ptr(this->camera->deriveViewMetrix()));
    this->shaderProgram->setUniformMat4("uProjection", glm::value_ptr(this->camera->deriveProjectionMatrix()));

    // Render cubes:
    this->cube->setPosition(glm::vec3(-1.0f, 0.0f, -1.0f))
        ->setRotation(.0f, glm::vec3(1.0f, .0f, .0f))
        ->render(this->shaderProgram);
    this->cube->setPosition(glm::vec3(2.0f, .0f, .0f))
        ->setRotation(.0f, glm::vec3(1.0f, .0f, .0f))
        ->render(this->shaderProgram);

    // Render floor:
    this->shaderProgram->setUniformI("uTexture", 1);
    this->floor->setPosition(glm::vec3(.0f, -.5f, .0f))
        ->setRotation(.0f, glm::vec3(1.0f, .0f, .0f))
        ->render(this->shaderProgram);

    // Bind back to default framebuffer and draw quad plane with attached framebuffer's color texture
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Avoid quad being depth test and discarded
    glDisable(GL_DEPTH_TEST);

    // clear all revelant buffer
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    this->frameBufferShaderProgram->use();
    this->frameBufferShaderProgram->setUniformI("uMode", this->uMode);
    glBindVertexArray(this->quadVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->textureColorBuffer);
    glDrawArrays(GL_TRIANGLES, 0, 6);

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

void _32_Postprocessing::cleanup() {
    MouseMoveEvent::dismiss(this->ctx->window);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(frameBufferVertexShader);
    glDeleteShader(frameBufferFragmentShader);

    this->shaderProgram->dispose();
    this->frameBufferShaderProgram->dispose();
    delete this->shaderProgram;
    delete this->frameBufferShaderProgram;
    delete this->textureCube;
    delete this->textureFloor;
    delete this->cube;
    delete this->floor;
}
