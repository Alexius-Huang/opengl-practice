#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aOffset;

out vec3 vColor;

uniform float uTime;

void main() {
    vec2 position = aPos * (sin(uTime + aOffset.x + aOffset.y) + 1.0) / 2.0;

    gl_Position = vec4(position + aOffset, .0, 1.0);
    vColor = aColor;
}
