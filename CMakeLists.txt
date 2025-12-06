cmake_minimum_required(VERSION 3.10)
project(OpenGLTutorial)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
    src/read-file.cpp
    src/read-shader-file.cpp
    src/shader-program.cpp
)

# Link libraries
target_link_libraries(opengl_tutorial PRIVATE glfw glm::glm glad OpenGL::GL)

# On macOS, we need to add the CoreFoundation framework
if (APPLE)
    target_link_libraries(opengl_tutorial PRIVATE "-framework CoreFoundation")
endif()
