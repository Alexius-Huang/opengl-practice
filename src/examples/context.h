#ifndef _TYPE_H
#define _TYPE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "../initialize.h"
#include "../helpers/read-shader-file.h"
#include "../helpers/shader-program.h"
#include "../helpers/close-window-on-esc-pressed.h"
#include "../helpers/toggle-polygon-mode-on-key-pressed.h"
#include "../helpers/gui.h"

typedef struct {
    GUI* gui;
    GLFWwindow* window;
    int selectedExampleIndex;
} Context;

#endif
