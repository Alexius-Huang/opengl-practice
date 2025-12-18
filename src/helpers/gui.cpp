#include "gui.h"

GUI::GUI(GLFWwindow* window) {
    if (this->isInitialized) throw runtime_error("ERROR::GUI::GUI_ALREADY_INITIALIZED");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    this->isInitialized = true;
}

void GUI::dispose() {
    if (!this->isInitialized) throw runtime_error("ERROR::GUI::GUI_DISPOSE_BUT_NOT_INITIALIZED");
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    this->isInitialized = false; 
}

const int MAX_VISIBLE_ITEMS = 8;
unsigned int GUI::render(unsigned int selectedIndex) {
    return this->render(selectedIndex, []() {});
}

unsigned int GUI::render(unsigned int selectedIndex, GuiCallback callback) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();

    ImGui::SetNextWindowPos(
        ImVec2(10.0f, 10.0f),
        ImGuiCond_Always
    );
    ImGui::SetNextWindowSizeConstraints(
        ImVec2(300, 0),          // min size
        ImVec2(FLT_MAX, 200)   // max size (height = 300 pixels)
    );

    ImGui::Begin("OpenGL Example");

    int selected = selectedIndex;
    float fullWidth = ImGui::GetContentRegionAvail().x;
    ImGui::SetNextItemWidth(fullWidth);

    bool changed = ImGui::ListBox(
        "##Sentences",
        &selected,
        [](void* data, int idx, const char** out_text) {
            auto* v = (std::vector<std::string>*)data;
            *out_text = (*v)[idx].c_str();
            return true;
        },
        &(this->exampleTitles),
        this->exampleTitles.size(),
        MAX_VISIBLE_ITEMS
    );

    ImGui::End();

    callback();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    return selected;
}
