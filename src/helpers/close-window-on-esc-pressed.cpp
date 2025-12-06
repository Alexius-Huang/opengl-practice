#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "close-window-on-esc-pressed.h"

void closeWindowOnEscPressed(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
