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

# Add executable
add_executable(opengl_tutorial
    src/main.cpp
    src/initialize.cpp
    src/helpers/read-file.cpp
    src/helpers/read-shader-file.cpp
    src/helpers/shader-program.cpp
    src/helpers/close-window-on-esc-pressed.cpp
    src/examples/01-hello-world.cpp
)

# Link libraries
target_link_libraries(opengl_tutorial PRIVATE glfw glm::glm glad OpenGL::GL)

# On macOS, we need to add the CoreFoundation framework
if (APPLE)
    target_link_libraries(opengl_tutorial PRIVATE "-framework CoreFoundation")
endif()
