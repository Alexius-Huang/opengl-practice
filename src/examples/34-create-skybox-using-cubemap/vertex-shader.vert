#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

// Transform using model, view and projection matrices
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

out vec2 vTexCoords;

void main() {
    mat4 transform = uProjection * uView * uModel;
    gl_Position = transform * vec4(aPos, 1.0);
    vTexCoords = aTexCoords;
}
