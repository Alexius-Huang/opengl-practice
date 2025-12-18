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
#include <functional>

using namespace std;

typedef function<void()> GuiCallback;

class GUI {
private:
    bool isInitialized = false;
    vector<string> exampleTitles;
public:
    GUI(GLFWwindow* window);

    void setExampleTitles(vector<string>& titles) {
        this->exampleTitles = titles;
    }

    // void init(GLFWwindow* window, vector<string>& exampleTitles);
    unsigned int render(unsigned int selectedIndex);
    unsigned int render(unsigned int selectedIndex, GuiCallback callback);
    void dispose();
};

#endif // GUI_H
