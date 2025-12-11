#version 330 core
out vec4 FragColor;

in vec4 vColor;
in vec2 vTexCoord;

// Using uniform to load texture data
uniform sampler2D uTexture;

void main() {
    // use texture function to render texture
    FragColor = texture(uTexture, vTexCoord);
}
