#ifndef _MOUSE_MOVE_EVENT_H
#define _MOUSE_MOVE_EVENT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace MouseMoveEvent {
    extern float lastX;
    extern float lastY;

    extern float offsetX;
    extern float offsetY;

    extern bool isFirstEvent;

    void reset();
    void clearOffset();
    void onMouseMove(GLFWwindow* window, double xPos, double yPos);
    void listen(GLFWwindow* window);
    void dismiss(GLFWwindow* window);
}

#endif 
