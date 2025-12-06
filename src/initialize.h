#include <GLFW/glfw3.h>

#ifndef INITIALIZE_H
#define INITIALIZE_H

struct OpenGLVersion {
    int major;
    int minor;
};

struct OpenGLWindowSize {
    int width;
    int height;
};

GLFWwindow* initialize(
    OpenGLVersion version,
    OpenGLWindowSize windowSize
);

#endif // INITIALIZE_H
