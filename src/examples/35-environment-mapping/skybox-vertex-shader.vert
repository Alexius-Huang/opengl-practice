#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 vTexCoords;

uniform mat4 uProjection;
uniform mat4 uView;

void main() {
    vTexCoords = aPos;
    vec4 position = uProjection * uView * vec4(aPos, 1.0);

    // A minor performance boost on perspective division
    // normalize the depth so it will always become z = 1.0
    // this will only be rendered if there are no objects visible in front of skybox
    // But we also need to change depth test function from GL_LESS to GL_LEQUAL
    gl_Position = position.xyww;
}
