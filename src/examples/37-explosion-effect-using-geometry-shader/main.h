#ifndef _37_EXPLOSION_EFFECT_USING_GEOMETRY_SHADER_H
#define _37_EXPLOSION_EFFECT_USING_GEOMETRY_SHADER_H

#include "../includes.h"

class _37_ExplosionEffectUsingGeometryShader : public Example {
private:
    unsigned int vertexShader;
    unsigned int fragmentShader;
    unsigned int geometryShader;
    ShaderProgram* shaderProgram;

    PerspectiveCamera* camera;

    bool isPressingTab = false;
    bool isListeningMouseEvent = false;

    Model* model;

public:
    _37_ExplosionEffectUsingGeometryShader(Context* ctx) : Example("Explosion Effect Using Geometry Shader", ctx) {}

    ~_37_ExplosionEffectUsingGeometryShader() {
        delete this->shaderProgram;
    }

    void setup() override;
    void render() override;
    void cleanup() override;
};

#endif  // _37_EXPLOSION_EFFECT_USING_GEOMETRY_SHADER_H
