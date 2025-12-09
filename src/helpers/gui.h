#ifndef GUI_H
#define GUI_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <stdexcept>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

class GUI {
private:
    bool isInitialized = false;
public:
    void init(GLFWwindow* window);
    void render();
    void dispose();
};

#endif // GUI_H
