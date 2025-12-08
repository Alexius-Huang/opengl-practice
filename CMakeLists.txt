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
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(opengl_practice ${SOURCES})

# Link libraries
target_link_libraries(opengl_practice PRIVATE glfw glm::glm glad OpenGL::GL)

# On macOS, we need to add the CoreFoundation framework
if (APPLE)
    target_link_libraries(opengl_practice PRIVATE "-framework CoreFoundation")
endif()
