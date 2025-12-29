#include "mouse-move-event.h"
#include <iostream>

namespace MouseMoveEvent {
    float lastX = 0.0f;
    float lastY = 0.0f;
    float offsetX = 0.0f;
    float offsetY = 0.0f;
    bool isFirstEvent = true;
}

void MouseMoveEvent::reset() {
    lastX = .0f;
    lastY = .0f;
    offsetX = .0f;
    offsetY = .0f;
    isFirstEvent = false;
}

void MouseMoveEvent::onMouseMove(GLFWwindow* window, double xPos, double yPos) {
    if (isFirstEvent) {
        lastX = xPos;
        lastY = yPos;
        isFirstEvent = false;
        return;
    }

    const float sensitivity = .1f;

    // Offset Y is reversed as y-coord range from bottom to top
    offsetX = (xPos - lastX) * sensitivity;
    offsetY = ((yPos - lastY) * -1) * sensitivity;
    lastX = xPos;
    lastY = yPos;
};

void MouseMoveEvent::listen(GLFWwindow* window) {
    isFirstEvent = true;
    glfwSetInputMode(
        window,
        GLFW_CURSOR,
        GLFW_CURSOR_DISABLED
    );
    glfwSetCursorPosCallback(window, onMouseMove);
}

void MouseMoveEvent::dismiss(GLFWwindow* window) {
    glfwSetInputMode(
        window,
        GLFW_CURSOR,
        GLFW_CURSOR_NORMAL
    );
    glfwSetCursorPosCallback(window, nullptr);
}

void MouseMoveEvent::clearOffset() {
    offsetX = .0f;
    offsetY = .0f;
}
