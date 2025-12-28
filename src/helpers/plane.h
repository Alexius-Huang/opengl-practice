#ifndef _PLANE_H
#define _PLANE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader-program.h"

// GLM - OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Plane {
private:
    inline static const float VERTICES[30] = {
        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
         1.0f, .0f,  1.0f,  2.0f, 0.0f,
        -1.0f, .0f,  1.0f,  0.0f, 0.0f,
        -1.0f, .0f, -1.0f,  0.0f, 2.0f,

         1.0f, .0f,  1.0f,  2.0f, 0.0f,
        -1.0f, .0f, -1.0f,  0.0f, 2.0f,
         1.0f, .0f, -1.0f,  2.0f, 2.0f								
    };
    GLuint VAO;
    GLuint VBO;

    glm::vec3 position = glm::vec3(.0f);
    glm::vec3 scale = glm::vec3(1.0f);

    float rotateAngle = .0f;
    glm::vec3 rotateAxis = glm::vec3(1.0f, .0f, .0f);

    glm::mat4 deriveModelMatrix();

    float size;

public:
    Plane(float size = 1.0f);
    ~Plane();

    Plane* setPosition(glm::vec3 p);
    Plane* setScale(glm::vec3 s);
    Plane* setRotation(float angle, glm::vec3 axis);

    void render(ShaderProgram* program);
    void render(ShaderProgram* program, const char* modelUniformName);
};

#endif
