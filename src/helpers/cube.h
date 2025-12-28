#ifndef _CUBE_H
#define _CUBE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader-program.h"

// GLM - OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Cube {
private:
    inline static const float VERTICES[288] = {
        // Vertices          // Texture Coords  // normal vector
        // Back Face
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,        0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,        0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,        0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,        0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,        0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,        0.0f,  0.0f, -1.0f,

        // Front Face
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,        0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,        0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,        0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,        0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,        0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,        0.0f,  0.0f,  1.0f,

        // Left face
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,       -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,       -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,       -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,       -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,       -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,       -1.0f,  0.0f,  0.0f,
        
        // Right face
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,        1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,        1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,        1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,        1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,        1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,        1.0f,  0.0f,  0.0f,
        
        // Bottom Face
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,        0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,        0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,        0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,        0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,        0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,        0.0f, -1.0f,  0.0f,

        // Top Face
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,        0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,        0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,        0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,        0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,        0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,        0.0f,  1.0f,  0.0f
    };
    GLuint VAO;
    GLuint VBO;

    glm::vec3 position = glm::vec3(.0f);
    glm::vec3 scale = glm::vec3(1.0f);

    float rotateAngle = .0f;
    glm::vec3 rotateAxis = glm::vec3(1.0f, .0f, .0f);

    glm::mat4 deriveModelMatrix();

public:
    Cube();
    ~Cube();

    Cube* setPosition(glm::vec3 p);
    Cube* setScale(glm::vec3 s);
    Cube* setRotation(float angle, glm::vec3 axis);

    void render(ShaderProgram* program);
    void render(ShaderProgram* program, const char* modelUniformName);
};

#endif