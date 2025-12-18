#version 330 core
out vec4 FragColor;

uniform vec3 uObjectColor;
uniform vec3 uLightColor;

// Phong Lighting System
uniform float uAmbientStrength;

void main() {
    vec3 ambient = uLightColor * uAmbientStrength;
    vec3 objectColor = uObjectColor * ambient;

    FragColor = vec4(objectColor, 1.0);
}
