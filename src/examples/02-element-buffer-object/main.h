#ifndef _02_ELEMENT_BUFFER_OBJECT_H
#define _02_ELEMENT_BUFFER_OBJECT_H

#include "../includes.h"

extern string _02_title;

void _02_elementBufferObject(Context *ctx);

class _02_ElementBufferObject : public Example {
private:
    unsigned int vertexShader;
    unsigned int fragmentShader;
    ShaderProgram *shaderProgram;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

public:
    _02_ElementBufferObject(Context *ctx) : Example("Element Buffer Object", ctx) {}

    ~_02_ElementBufferObject() { delete this->shaderProgram; }

    void setup() override;
    void render() override;
    void cleanup() override;
};

#endif // _02_ELEMENT_BUFFER_OBJECT_H
