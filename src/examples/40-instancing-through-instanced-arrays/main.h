#ifndef _40_INSTANCING_THROUGH_INSTANCED_ARRAYS_H
#define _40_INSTANCING_THROUGH_INSTANCED_ARRAYS_H

#include "../includes.h"

class _40_InstancingThroughInstancedArrays : public Example {
private:
    unsigned int vertexShader;
    unsigned int fragmentShader;
    ShaderProgram* shaderProgram;
    unsigned int VAO;
    unsigned int VBO;

    unsigned int instancedVBO;

    const float VERTICES[30] {
        // positions     // colors
        -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
         0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
        -0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

        -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
         0.05f, -0.05f,  0.0f, 1.0f, 0.0f,   
         0.05f,  0.05f,  0.0f, 1.0f, 1.0f		    		
    };

public:
    _40_InstancingThroughInstancedArrays(Context* ctx): Example("Instancing Through Instanced Arrays", ctx) {}

    ~_40_InstancingThroughInstancedArrays() {
        delete this->shaderProgram;
    }

    void setup() override;
    void render() override;
    void cleanup() override;
};

#endif // _40_INSTANCING_THROUGH_INSTANCED_ARRAYS_H
