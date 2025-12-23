#include "mesh.h"

Mesh::Mesh(
    std::vector<Vertex> vertices,
    std::vector<unsigned int> indices,
    std::vector<Texture> textures
): vertices(vertices), indices(indices), textures(textures) {
    this->setupMesh();
};

void Mesh::setupMesh() {
    glGenVertexArrays(1, &(this->VAO));
    glBindVertexArray(this->VAO);

    glGenBuffers(1, &(this->EBO));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    // TODO: this is currently using vector, clarify why using &indices[0]
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &(this->indices)[0], GL_STATIC_DRAW);

    glGenBuffers(1, &(this->VBO));
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &(this->vertices)[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Mesh::render(ShaderProgram* program) {
    program->use();

    // We do not know how many texture a model would have, so we enforce a convention
    // where model shader should have their texture name to be texture_diffuse / texture_specular
    unsigned int diffuseTexNum = 1;
    unsigned int specularTexNum = 1;
    for (size_t i = 0; i < this->textures.size(); i++) {
        std::string number;
        std::string name = this->textures[i].type;

        if (name == "texture_diffuse") {
            number = std::to_string(diffuseTexNum++);
        } else if (name == "texture_specular") {
            number = std::to_string(specularTexNum++);
        }

        program->setUniformI(("material." + name + number).c_str(), i);

        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
    }

    glActiveTexture(0);

    // Draw Mesh
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glDrawElements(
        GL_TRIANGLES,
        this->indices.size(),
        GL_UNSIGNED_INT,
        0
    );

    glBindVertexArray(0);
}
