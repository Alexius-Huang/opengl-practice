#include "toggle-polygon-mode-on-key-pressed.h"

bool __TOGGLE_POLYGON_KEY_PRESSING = false;

void togglePolygonModeOnKeyPressed(GLFWwindow *window, unsigned int key) {
    if (glfwGetKey(window, key) == GLFW_PRESS) {
        if (!__TOGGLE_POLYGON_KEY_PRESSING) {
            GLint polygonModes[2];
            glGetIntegerv(GL_POLYGON_MODE, polygonModes);

            GLint polygonMode = polygonModes[0] == GL_FILL ? GL_FILL : GL_LINE;

            polygonMode = polygonMode == GL_FILL ? GL_LINE : GL_FILL;
            glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
            __TOGGLE_POLYGON_KEY_PRESSING = true;
        }
    } else {
        __TOGGLE_POLYGON_KEY_PRESSING = false;
    }
}
