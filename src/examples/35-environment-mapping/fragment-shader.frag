#version 330 core
out vec4 FragColor;

uniform sampler2D uTexture;
uniform samplerCube uSkyboxTexture;
uniform vec3 uCameraPosition;

in vec2 vTexCoords;
in vec3 vNormal;
in vec3 vFragmentPosition;

void main() {
    vec3 reflectIncident = normalize(vFragmentPosition - uCameraPosition);
    vec3 reflection = reflect(reflectIncident, normalize(vNormal));
    FragColor = texture(uSkyboxTexture, reflection);
}
