#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vColor;

uniform vec2 uTranslationOffsets[100];

void main() {
    vec2 translationOffset = uTranslationOffsets[gl_InstanceID];
    gl_Position = vec4(aPos + translationOffset, .0, 1.0);
    vColor = aColor;
}
