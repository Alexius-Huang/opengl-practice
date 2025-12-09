#ifndef _CONTEXT_H
#define _CONTEXT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../helpers/gui.h"

typedef struct {
    GUI* gui;
    GLFWwindow* window;
    int selectedExampleIndex;
    int totalExamplesCount;
} Context;

#endif
