#version 330 core
out vec4 FragColor;

in vec4 vColor;
in vec2 vTexCoord;

// Using uniform to load multiple textures data
uniform sampler2D uTexture1;
uniform sampler2D uTexture2;
uniform float uMixPercentage;

void main() {
    // use texture function to render texture
    FragColor = mix(
        texture(uTexture1, vTexCoord),
        texture(uTexture2, vTexCoord),
        uMixPercentage
    );
}
