#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "close-window-on-esc-pressed.h"

bool closeWindowOnEscPressed(GLFWwindow *window) {
    return glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
}
