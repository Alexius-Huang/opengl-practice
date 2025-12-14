#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 vColor;
out vec2 vTexCoord;

// Transform using model, view and projection matrices
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main() {
    mat4 transform = uProjection * uView * uModel;

    gl_Position = transform * vec4(aPos, 1.0);

    vColor = vec4(aColor, 1.0);
    vTexCoord = aTexCoord;
}
