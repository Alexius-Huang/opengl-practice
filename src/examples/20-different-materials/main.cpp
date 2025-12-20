#include "main.h"
#include <iostream>
using namespace std;

namespace _20_MouseEvent {
    float lastX = .0f;
    float lastY = .0f;

    float offsetX = .0f;
    float offsetY = .0f;

    bool isCapturingEvent = false;
    bool isFirstEvent = false;
}

namespace _20_ScrollEvent {
    // Camera's field of view, useful for implementing zoom feature
    float fov = 45.0f;
}

namespace _20_Config {
    MaterialType selectedMaterialType { MaterialType::Emerald };
}

void _20_onMouseMove(GLFWwindow* window, double xPos, double yPos) {
    if (_20_MouseEvent::isFirstEvent) {
        _20_MouseEvent::lastX = xPos;
        _20_MouseEvent::lastY = yPos;
        _20_MouseEvent::isFirstEvent = false;
        return;
    }

    // Offset Y is reversed as y-coord range from bottom to top
    float offsetX = xPos - _20_MouseEvent::lastX;
    float offsetY = (yPos - _20_MouseEvent::lastY) * -1;

    const float sensitivity = .1f;
    _20_MouseEvent::offsetX = offsetX * sensitivity;
    _20_MouseEvent::offsetY = offsetY * sensitivity;
    _20_MouseEvent::lastX = xPos;
    _20_MouseEvent::lastY = yPos;
};

void _20_onScroll(GLFWwindow* window, double xOffset, double yOffset) {
    _20_ScrollEvent::fov -= (float)yOffset;

    if (_20_ScrollEvent::fov < 1.0f) {
        _20_ScrollEvent::fov = 1.0f;
    } else if (_20_ScrollEvent::fov > 45.0f) {
        _20_ScrollEvent::fov = 45.0f;
    }
}

void _20_DifferentMaterials::setup() {
    this->cube = new Cube;
    this->camera = new PerspectiveCamera(
        this->ctx->window,
        glm::vec3(-1.7f, -1.25f, 2.5f),
        25.9f,
        -35.0f,
        _20_ScrollEvent::fov,
        .1,
        2000.0f
    );

    glm::mat4 view = this->camera->deriveViewMetrix();
    glm::mat4 projection = this->camera->deriveProjectionMatrix();

    this->vertexShader = readShaderFile("./src/examples/20-different-materials/vertex-shader.vert");
    this->fragmentShader = readShaderFile("./src/examples/20-different-materials/fragment-shader.frag");
    this->lightFragmentShader = readShaderFile("./src/examples/20-different-materials/light-shader.frag");

    this->shaderProgram = new ShaderProgram;
    this->shaderProgram->attachShader(vertexShader);
    this->shaderProgram->attachShader(fragmentShader);
    this->shaderProgram->link();
    this->shaderProgram->use();
    this->shaderProgram->setUniformVec3("uLight.position", glm::value_ptr(this->lightPosition));
    this->shaderProgram->setUniformVec3("uLight.ambient", glm::value_ptr(glm::vec3(.2f, .2f, .2f)));
    this->shaderProgram->setUniformVec3("uLight.diffuse", glm::value_ptr(glm::vec3(.5f, .5f, .5f)));
    this->shaderProgram->setUniformVec3("uLight.specular", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));

    this->shaderProgram->setUniformVec3("uViewPosition", glm::value_ptr(this->camera->position));

    MATERIALS.at(_20_Config::selectedMaterialType).attachToShaderProgram(shaderProgram);

    this->shaderProgram->setUniformMat4("uView", glm::value_ptr(view));
    this->shaderProgram->setUniformMat4("uProjection", glm::value_ptr(projection));

    this->lightShaderProgram = new ShaderProgram;
    this->lightShaderProgram->attachShader(vertexShader);
    this->lightShaderProgram->attachShader(lightFragmentShader);
    this->lightShaderProgram->link();
    this->lightShaderProgram->use();
    this->lightShaderProgram->setUniformVec3("uLightColor", glm::value_ptr(this->lightColor));
    this->lightShaderProgram->setUniformMat4("uView", glm::value_ptr(view));
    this->lightShaderProgram->setUniformMat4("uProjection", glm::value_ptr(projection));

    glUseProgram(0);

    glEnable(GL_DEPTH_TEST);
}

void _20_DifferentMaterials::render() {
    if (closeWindowOnEscPressed(ctx->window)) {
        this->setShouldExit(true);
        glfwSetWindowShouldClose(ctx->window, true);
        return;
    };

    if (glfwGetKey(this->ctx->window, GLFW_KEY_1) == GLFW_PRESS) {
        // Prevent from long press and capture single first Tab press
        if (!this->isPressingTab) {
            this->isPressingTab = true;
            _20_MouseEvent::isCapturingEvent = !_20_MouseEvent::isCapturingEvent;
            glfwSetInputMode(
                this->ctx->window,
                GLFW_CURSOR,
                _20_MouseEvent::isCapturingEvent ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL
            );

            glfwSetCursorPosCallback(
                this->ctx->window,
                _20_MouseEvent::isCapturingEvent ? _20_onMouseMove : nullptr
            );

            glfwSetScrollCallback(
                this->ctx->window,
                _20_MouseEvent::isCapturingEvent ? _20_onScroll : nullptr
            );

            if (_20_MouseEvent::isCapturingEvent) {
                _20_MouseEvent::isFirstEvent = true;
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
    float newPitch = this->camera->getPitch() + _20_MouseEvent::offsetY;
    this->camera->setPitch(newPitch);
    this->camera->yaw += _20_MouseEvent::offsetX;

    // reset to prevent from using the current offset to move the pitch / yaw in next frame
    _20_MouseEvent::offsetX = .0f;
    _20_MouseEvent::offsetY = .0f;

    this->camera->update(this->getDelta());

    glm::mat4 view = this->camera->deriveViewMetrix();
    glm::mat4 projection = this->camera->deriveProjectionMatrix();

    // Render object
    this->shaderProgram->use();
    this->shaderProgram->setUniformMat4("uProjection", glm::value_ptr(projection));
    this->shaderProgram->setUniformMat4("uView", glm::value_ptr(view));
    this->shaderProgram->setUniformVec3("uViewPosition", glm::value_ptr(this->camera->position));
    MATERIALS.at(_20_Config::selectedMaterialType).attachToShaderProgram(shaderProgram);


    this->cube
        ->setPosition(this->objectPosition)
        ->setScale(this->objectScale)
        ->render(this->shaderProgram);

    // Render light source
    this->lightShaderProgram->use();
    this->lightShaderProgram->setUniformMat4("uProjecton", glm::value_ptr(projection));
    this->lightShaderProgram->setUniformMat4("uView", glm::value_ptr(view));
    this->lightShaderProgram->setUniformVec3("uLightColor", glm::value_ptr(this->lightColor));
    this->cube
        ->setPosition(this->lightPosition)
        ->setScale(this->lightScale)
        ->render(this->lightShaderProgram);

    int index = ctx->gui->render(ctx->selectedExampleIndex, []() {
        ImGui::SetNextWindowPos(
            ImVec2(10.0f, 200.0f),
            ImGuiCond_Always
        );
        ImGui::SetNextWindowSizeConstraints(
            ImVec2(300, 0),        // min size
            ImVec2(300, 300)   // max size (height = 300 pixels)
        );

        ImGui::Begin("Materials");

        vector<string> titles;
        for (MaterialType m : MATERIAL_TYPES) {
            titles.push_back(
                MATERIALS.at(m).getName()
            );
        }

        int selected = (int)_20_Config::selectedMaterialType;
        bool changed = ImGui::ListBox(
            "##Sentences",
            &selected,
            [](void* data, int idx, const char** out_text) {
                auto* v = (std::vector<std::string>*)data;
                *out_text = (*v)[idx].c_str();
                return true;
            },
            &titles,
            titles.size(),
            5
        );

        if (changed) {
            _20_Config::selectedMaterialType = (MaterialType)selected;
        }

        ImGui::End();
    });
    if (index != ctx->selectedExampleIndex) {
        ctx->selectedExampleIndex = index;
        this->setShouldExit(true);
        return;
    }

    glfwSwapBuffers(ctx->window);
    glfwPollEvents();
}

void _20_DifferentMaterials::cleanup() {
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(lightFragmentShader);

    this->shaderProgram->dispose();
    this->lightShaderProgram->dispose();
    delete this->shaderProgram;
    delete this->lightShaderProgram;
    delete this->cube;
    delete this->camera;
}
