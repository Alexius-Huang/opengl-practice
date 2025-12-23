#ifndef _MODEL_H
#define _MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include "stb-image.h"

#include "mesh.h"

class Model {
private:
    std::vector<Mesh> meshes;
    std::string directory;

    // Since texture loading is a very heavy operation, we store the loaded
    // texture globally
    std::vector<Texture> loadedTextures;

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(
        aiMaterial* material,
        aiTextureType type,
        std::string typeName
    );

public:
    Model(char *path) {
        this->loadModel(path);
    }

    void render(ShaderProgram* program) {
        for (auto mesh : this->meshes) {
            mesh.render(program);
        }
    };
};

#endif