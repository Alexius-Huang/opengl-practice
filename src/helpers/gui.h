#ifndef GUI_H
#define GUI_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <stdexcept>

class GUI {
private:
    bool isInitialized = false;
public:
    void init(GLFWwindow* window);
    void render();
    void dispose();
};

#endif // GUI_H
