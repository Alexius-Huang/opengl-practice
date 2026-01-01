#ifndef _39_INSTANCING_H
#define _39_INSTANCING_H

#include "../includes.h"

class _39_Instancing : public Example {
private:
    unsigned int vertexShader;
    unsigned int fragmentShader;
    ShaderProgram* shaderProgram;
    unsigned int VAO;
    unsigned int VBO;

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
    _39_Instancing(Context* ctx): Example("Instancing", ctx) {}

    ~_39_Instancing() {
        delete this->shaderProgram;
    }

    void setup() override;
    void render() override;
    void cleanup() override;
};

#endif // _39_INSTANCING_H
