#ifndef _PERSPECTIVE_CAMERA_H
#define _PERSPECTIVE_CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class PerspectiveCamera {
private:
    
public:
    float yaw;
    float pitch;
    float fov;
    float near;
    float far;
    GLFWwindow* window;
    glm::vec3 position;

    PerspectiveCamera(
        GLFWwindow* window,
        glm::vec3 position,
        float pitch,
        float yaw,
        float fov,
        float near,
        float far
    ):
        window(window),
        position(position),
        pitch(pitch),
        yaw(yaw),
        fov(fov),
        near(near),
        far(far)
    {}

    void setPitch(float pitch) {
        this->pitch = pitch;

        // Constraint pitch to not be able to pitch backward
        if (this->pitch > 89.0f) {
            this->pitch = 89.0f;
        } else if (this->pitch < -89.0f) {
            this->pitch = -89.0f;
        }
    }

    float getPitch() { return this->pitch; }

    glm::vec3 deriveCameraFrontVector() {
        glm::vec3 direction = glm::vec3(
            cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch)),
            sin(glm::radians(this->pitch)),
            sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch))
        );

        return glm::normalize(direction);
    }

    glm::mat4 deriveProjectionMatrix() {
        float fov = glm::radians(this->fov);

        int w, h;
        glfwGetWindowSize(this->window, &w, &h);

        float aspectRatio = (float)w / (float)h;
        return glm::perspective(this->fov, aspectRatio, this->near, this->far);
    }

    glm::mat4 deriveViewMetrix() {
        // Gram-Schmidt process of deriving camera's coordinate
        glm::vec3 cameraFront = this->deriveCameraFrontVector();
        glm::vec3 up = glm::vec3(.0f, 1.0f, .0f);
        glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, up));
        glm::vec3 cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));

        return glm::lookAt(
            this->position,
            this->position + cameraFront,
            cameraUp
        );
    }

    // This should be called during rendering
    void update(float deltaTime) {
        float cameraSpeed = 2.5f;

        // Gram-Schmidt process of deriving camera's coordinate
        glm::vec3 cameraFront = this->deriveCameraFrontVector();
        glm::vec3 up = glm::vec3(.0f, 1.0f, .0f);
        glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, up));
        glm::vec3 cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));

        if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS) {
            this->position += cameraFront * cameraSpeed * deltaTime;
        }
        if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS) {
            this->position -= cameraFront * cameraSpeed * deltaTime;
        }
        if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS) {
            this->position += cameraRight * cameraSpeed * deltaTime;
        }
        if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS) {
            this->position -= cameraRight * cameraSpeed * deltaTime;
        }
    }
};

#endif
