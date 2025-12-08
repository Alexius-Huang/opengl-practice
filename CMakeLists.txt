cmake_minimum_required(VERSION 3.10)
project(OpenGLTutorial)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find GLFW
find_package(glfw3 REQUIRED)

# Find GLM
find_package(glm REQUIRED)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Add GLAD
add_library(glad STATIC libs/glad/src/glad.c)
target_include_directories(glad PUBLIC libs/glad)

# --- Dependency Management (FetchContent) ---
include(FetchContent)

# Declare ImGui dependency from GitHub
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.92.5 # Pin to a specific, stable version
)

# Make the dependency available
FetchContent_MakeAvailable(imgui)

# --- ImGui Build ---
# Define the source files for the ImGui library using the path from FetchContent
set(IMGUI_SOURCES
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

# Create a static library for ImGui
add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
target_link_libraries(imgui PRIVATE glfw)

# --- Main Application ---
# Find all the application's source files
file(GLOB_RECURSE APP_SOURCES "src/*.cpp")

# Add executable using the found source files
add_executable(opengl_practice ${APP_SOURCES})

# Link libraries
target_link_libraries(opengl_practice PRIVATE glfw glm::glm glad OpenGL::GL imgui)

# On macOS, we need to add the CoreFoundation framework
if (APPLE)
    target_link_libraries(opengl_practice PRIVATE "-framework CoreFoundation")
endif()
