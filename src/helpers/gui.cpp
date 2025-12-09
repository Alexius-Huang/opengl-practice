#include "gui.h"

void GUI::init(GLFWwindow* window) {
    if (isInitialized) throw std::runtime_error("ERROR::GUI::GUI_ALREADY_INITIALIZED");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    isInitialized = true;
}

void GUI::dispose() {
    if (!isInitialized) throw std::runtime_error("ERROR::GUI::GUI_DISPOSE_BUT_NOT_INITIALIZED");
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    isInitialized = false; 
}

void GUI::render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(
        ImVec2(10.0f, 10.0f),
        ImGuiCond_FirstUseEver
    );
    ImGui::Begin("Hello, world!");
    ImGui::Text("This is some useful text.");
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
