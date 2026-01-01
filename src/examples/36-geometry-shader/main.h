#ifndef _36_GEOMETRY_SHADER_H
#define _36_GEOMETRY_SHADER_H

#include "../includes.h"

class _36_GeometryShader : public Example {
private:
    unsigned int vertexShader;
    unsigned int fragmentShader;
    unsigned int geometryShader;
    ShaderProgram* shaderProgram;
    unsigned int VAO;
    unsigned int VBO;

    const float POINTS[20] {
        -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // top-left
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // top-right
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
        -0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // bottom-left
    };

public:
    _36_GeometryShader(Context* ctx): Example("Geometry Shader", ctx) {}

    ~_36_GeometryShader() {}

    void setup() override;
    void render() override;
    void cleanup() override;
};

#endif // _36_GEOMETRY_SHADER_H
