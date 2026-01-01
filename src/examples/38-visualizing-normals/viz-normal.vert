#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 vTexCoords;
out vec3 vNormal;

uniform mat4 uModel;
uniform mat4 uView;

void main() {
    vTexCoords = aTexCoords;

    mat3 normalMatrix = mat3(transpose(inverse(uView * uModel)));
    vNormal = normalize(vec3(vec4(normalMatrix * aNormal, .0)));

    // Using view space because in the geometry shader we are performing
    // geometric calculation, projection space is not linear in this case...
    gl_Position = uView * uModel * vec4(aPos, 1.0);
}
