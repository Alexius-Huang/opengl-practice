#include "plane.h"

Plane::Plane(float size): size(size) {
    glGenVertexArrays(1, &(this->VAO));
    glBindVertexArray(this->VAO);

    float vertices[36];

    for (int i = 0; i < 36; i++) {
        if ((i % 5) >= 3) {
            vertices[i] = Plane::VERTICES[i];
        } else {
            vertices[i] = Plane::VERTICES[i] * this->size;
        }
    }

    glGenBuffers(1, &(this->VBO));
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );

    // Selecting positions:
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Selecting texture coordinates:
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind everything before being used
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);    
}

Plane::~Plane() {
    glDeleteVertexArrays(1, &(this->VAO));
    glDeleteBuffers(1, &(this->VBO));
}

void Plane::render(ShaderProgram* program) {
    this->render(program, "uModel");
}

void Plane::render(ShaderProgram* program, const char* modelUniformName) {
    program->use();
    program->setUniformMat4(modelUniformName, glm::value_ptr(this->deriveModelMatrix()));

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBindVertexArray(this->VAO);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Unbind everything after being rendered
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    this->clearTransformation();
}

Plane* Plane::setPosition(glm::vec3 p) {
    this->position = p;
    return this;
}

Plane* Plane::setScale(glm::vec3 s) {
    this->scale = s;
    return this;
}

// TODO: currently we are using local space to perform transformation, it would
//       be better to convert using world space to control it
Plane* Plane::setRotation(float angle, glm::vec3 axis) {
    this->rotateAngles.push_back(angle);
    this->rotateAxis.push_back(axis);
    return this;
}

glm::mat4 Plane::deriveModelMatrix() {
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, this->position);    
    model = glm::scale(model, this->scale);

    for (size_t i = 0; i < this->rotateAngles.size(); i++) {
        model = glm::rotate(model, glm::radians(this->rotateAngles.at(i)), this->rotateAxis.at(i));
    }

    return model;
}
