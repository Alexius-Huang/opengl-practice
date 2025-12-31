#version 330 core
out vec4 FragColor;

in vec3 vTexCoords;

uniform samplerCube uSkyboxTexture;

void main() {    
    FragColor = texture(uSkyboxTexture, vTexCoords);
}
