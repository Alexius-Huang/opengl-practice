#include "cube.h"

Cube::Cube() {
    glGenVertexArrays(1, &(this->VAO));
    glBindVertexArray(this->VAO);

    glGenBuffers(1, &(this->VBO));
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(Cube::VERTICES),
        Cube::VERTICES,
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

Cube::~Cube() {
    glDeleteVertexArrays(1, &(this->VAO));
    glDeleteBuffers(1, &(this->VBO));
}

void Cube::render(ShaderProgram* program) {
    this->render(program, "uModel");
}

void Cube::render(ShaderProgram* program, const char* modelUniformName) {
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBindVertexArray(this->VAO);

    program->setUniformMat4(modelUniformName, glm::value_ptr(this->deriveModelMatrix()));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Unbind everything after being rendered
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Cube* Cube::setPosition(glm::vec3 p) {
    this->position = p;
    return this;
}

Cube* Cube::setScale(glm::vec3 s) {
    this->scale = s;
    return this;
}

Cube* Cube::setRotation(float angle, glm::vec3 axis) {
    this->rotateAngle = angle;
    this->rotateAxis = axis;
    return this;
}

glm::mat4 Cube::deriveModelMatrix() {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, this->position);
    model = glm::scale(model, this->scale);
    model = glm::rotate(model, glm::radians(this->rotateAngle), this->rotateAxis);
    return model;
}
