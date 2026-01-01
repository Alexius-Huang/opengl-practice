#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 vTexCoords;
out vec3 vNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main() {
    vTexCoords = aTexCoords;

    mat3 normalMatrix = mat3(transpose(inverse(uProjection * uView * uModel)));
    vNormal = normalize(vec3(vec4(normalMatrix * aNormal, .0)));

    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
}
