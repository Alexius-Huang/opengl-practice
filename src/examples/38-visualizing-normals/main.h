#ifndef _38_VISUALIZING_NORMALS_H
#define _38_VISUALIZING_NORMALS_H

#include "../includes.h"

class _38_VisualizingNormals : public Example {
private:
    unsigned int vertexShader;
    unsigned int fragmentShader;
    unsigned int geometryShader;
    ShaderProgram* shaderProgram;

    unsigned int vizNormalVertexShader;
    unsigned int vizNormalFragmentShader;
    unsigned int vizNormalGeometryShader;
    ShaderProgram* vizNormalShaderProgram;

    PerspectiveCamera* camera;

    bool isPressingTab = false;
    bool isListeningMouseEvent = false;

    Model* model;

public:
    _38_VisualizingNormals(Context* ctx) : Example("Visualizing Normals", ctx) {}

    ~_38_VisualizingNormals() {
        delete this->shaderProgram;
    }

    void setup() override;
    void render() override;
    void cleanup() override;
};

#endif  // _38_VISUALIZING_NORMALS_H
