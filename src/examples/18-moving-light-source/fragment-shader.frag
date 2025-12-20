#version 330 core
out vec4 FragColor;

uniform vec3 uObjectColor;
uniform vec3 uLightColor;
uniform vec3 uLightPosition;
uniform vec3 uViewPosition;

uniform float uAmbientStrength;

uniform int uSpecularPower;
uniform float uSpecularStrength;

in vec3 vNormal;
in vec3 vFragmentPosition;

void main() {
    // Deriving ambient component
    vec3 ambient = uLightColor * uAmbientStrength;

    // Deriving diffuse component
    vec3 norm = normalize(vNormal);
    vec3 lightDirection = normalize(uLightPosition - vFragmentPosition);
    vec3 diffuse = max(dot(norm, lightDirection), 0.0) * uLightColor;

    // Derive specular component
    vec3 viewDirection = normalize(uViewPosition - vFragmentPosition);
    vec3 reflected = reflect(-lightDirection, norm);
    vec3 spectacle = pow(max(dot(reflected, viewDirection), 0.0), uSpecularPower) * uLightColor * uSpecularStrength;

    // Phong Lighting System is the sum of ambient, diffuse and spectacle
    vec3 objectColor = uObjectColor * (ambient + diffuse + spectacle);
    FragColor = vec4(objectColor, 1.0);
}
