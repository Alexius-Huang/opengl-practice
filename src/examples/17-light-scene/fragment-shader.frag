#version 330 core
out vec4 FragColor;

uniform vec3 uObjectColor;
uniform vec3 uLightColor;
uniform vec3 uLightPosition;

uniform float uAmbientStrength;

in vec3 vNormal;
in vec3 vFragmentPosition;

void main() {
    // Deriving ambient component
    vec3 ambient = uLightColor * uAmbientStrength;

    // Deriving diffuse component
    vec3 norm = normalize(vNormal);
    vec3 lightDirection = normalize(uLightPosition - vFragmentPosition);
    vec3 diffuse = max(dot(norm, lightDirection), 0.0) * uLightColor;

    // Phong Lighting System is the sum of ambient, diffuse and spectacle
    vec3 objectColor = uObjectColor * (ambient + diffuse);
    FragColor = vec4(objectColor, 1.0);
}
