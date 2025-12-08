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

# --- ImGui Build ---
# Define the source files for the ImGui library
set(IMGUI_SOURCES
    libs/imgui/imgui.cpp
    libs/imgui/imgui_draw.cpp
    libs/imgui/imgui_tables.cpp
    libs/imgui/imgui_widgets.cpp
    libs/imgui/backends/imgui_impl_glfw.cpp
    libs/imgui/backends/imgui_impl_opengl3.cpp
)

# Create a static library for ImGui
add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC libs/imgui libs/imgui/backends)
target_link_libraries(imgui PRIVATE glfw)

# --- Main Application ---
# Find all the application's source files
file(GLOB_RECURSE APP_SOURCES "src/*.cpp")

# Add executable using the found source files
add_executable(opengl_tutorial ${APP_SOURCES})

# Link libraries
target_link_libraries(opengl_tutorial PRIVATE glfw glm::glm glad OpenGL::GL imgui)

# On macOS, we need to add the CoreFoundation framework
if (APPLE)
    target_link_libraries(opengl_tutorial PRIVATE "-framework CoreFoundation")
endif()
