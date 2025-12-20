#ifndef _MATERIAL_H
#define _MATERIAL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <unordered_map>
#include <array>
#include <vector>

#include "shader-program.h"

enum class MaterialType {
    Emerald,
    Jade,
    Obsidian,
    Pearl,
    Ruby,
    Turquoise,
    Brass,
    Bronze,
    Chrome,
    Copper,
    Gold,
    Silver,
    __COUNT__
};

class Material {
private:
    string name;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

public:
    Material(
        string name,
        glm::vec3 ambient,
        glm::vec3 diffuse,
        glm::vec3 specular,
        float shininess
    ):
        name(name),
        ambient(ambient),
        diffuse(diffuse),
        specular(specular),
        shininess(shininess)
    {}

    string getName() const { return this->name; }

    void attachToShaderProgram(ShaderProgram* program) const {
        program->setUniformVec3("uMaterial.ambient", glm::value_ptr(this->ambient));
        program->setUniformVec3("uMaterial.diffuse", glm::value_ptr(this->diffuse));
        program->setUniformVec3("uMaterial.specular", glm::value_ptr(this->specular));
        program->setUniformF("uMaterial.shininess", this->shininess);
    }
};

constexpr std::array<MaterialType, static_cast<size_t>(MaterialType::__COUNT__)> MATERIAL_TYPES = {
    MaterialType::Emerald,
    MaterialType::Jade,
    MaterialType::Obsidian,
    MaterialType::Pearl,
    MaterialType::Ruby,
    MaterialType::Turquoise,
    MaterialType::Brass,
    MaterialType::Bronze,
    MaterialType::Chrome,
    MaterialType::Copper,
    MaterialType::Gold,
    MaterialType::Silver
};

const std::unordered_map<MaterialType, Material> MATERIALS = {
    {
        MaterialType::Emerald,
        Material(
            "Emerald",
            glm::vec3(0.0215f, 0.1745f, 0.0215f),
            glm::vec3(0.07568f, 0.61424f, 0.07568f),
            glm::vec3(0.633f, 0.727811f, 0.633f),
            76.8f
        )
    },
    {
        MaterialType::Jade,
        Material(
            "Jade",
            glm::vec3(0.135f, 0.2225f, 0.1575f),
            glm::vec3(0.54f, 0.89f, 0.63f),
            glm::vec3(0.316228f, 0.316228f, 0.316228f),
            12.8f
        )
    },
    {
        MaterialType::Obsidian,
        Material(
            "Obsidian",
            glm::vec3(0.05375f, 0.05f, 0.06625f),
            glm::vec3(0.18275f, 0.17f, 0.22525f),
            glm::vec3(0.332741f, 0.328634f, 0.346435f),
            38.4f
        )
    },
    {
        MaterialType::Pearl,
        Material(
            "Pearl",
            glm::vec3(0.25f, 0.20725f, 0.20725f),
            glm::vec3(1.0f, 0.829f, 0.829f),
            glm::vec3(0.296648f, 0.296648f, 0.296648f),
            11.264f
        )
    },
    {
        MaterialType::Ruby,
        Material(
            "Ruby",
            glm::vec3(0.1745f, 0.01175f, 0.01175f),
            glm::vec3(0.61424f, 0.04136f, 0.04136f),
            glm::vec3(0.727811f, 0.626959f, 0.626959f),
            76.8f
        )
    },
    {
        MaterialType::Turquoise,
        Material(
            "Turquoise",
            glm::vec3(0.1f, 0.18725f, 0.1745f),
            glm::vec3(0.396f, 0.74151f, 0.69102f),
            glm::vec3(0.297254f, 0.30829f, 0.306678f),
            12.8f
        )
    },
    {
        MaterialType::Brass,
        Material(
            "Brass",
            glm::vec3(0.329412f, 0.223529f, 0.027451f),
            glm::vec3(0.780392f, 0.568627f, 0.113725f),
            glm::vec3(0.992157f, 0.941176f, 0.807843f),
            27.8974f
        )
    },
    {
        MaterialType::Bronze,
        Material(
            "Bronze",
            glm::vec3(0.2125f, 0.1275f, 0.054f),
            glm::vec3(0.714f, 0.4284f, 0.18144f),
            glm::vec3(0.393548f, 0.271906f, 0.166721f),
            25.6f
        )
    },
    {
        MaterialType::Chrome,
        Material(
            "Chrome",
            glm::vec3(0.25f, 0.25f, 0.25f),
            glm::vec3(0.4f, 0.4f, 0.4f),
            glm::vec3(0.774597f, 0.774597f, 0.774597f),
            76.8f
        )
    },
    {
        MaterialType::Copper,
        Material(
            "Copper",
            glm::vec3(0.19125f, 0.0735f, 0.0225f),
            glm::vec3(0.7038f, 0.27048f, 0.0828f),
            glm::vec3(0.256777f, 0.137622f, 0.086014f),
            12.8f
        )
    },
    {
        MaterialType::Gold,
        Material(
            "Gold",
            glm::vec3(0.24725f, 0.1995f, 0.0745f),
            glm::vec3(0.75164f, 0.60648f, 0.22648f),
            glm::vec3(0.628281f, 0.555802f, 0.366065f),
            51.2f
        )
    },
    {
        MaterialType::Silver,
        Material(
            "Silver",
            glm::vec3(0.19225f, 0.19225f, 0.19225f),
            glm::vec3(0.50754f, 0.50754f, 0.50754f),
            glm::vec3(0.508273f, 0.508273f, 0.508273f),
            51.2f
        )
    }
};


#endif
