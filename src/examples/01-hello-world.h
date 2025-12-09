#ifndef _01_HELLO_WORLD_H
#define _01_HELLO_WORLD_H

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

void _01_helloWorld(GUI* gui);

#endif // _01_HELLO_WORLD_H
