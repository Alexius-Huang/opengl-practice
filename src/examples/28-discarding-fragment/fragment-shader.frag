#version 330 core
out vec4 FragColor;

in vec2 vTexCoord;
uniform sampler2D uTexture;

void main() {
    vec4 textureColor = texture(uTexture, vTexCoord);

    // Alpha channel, we discard fragment when alpha is low
    if (textureColor.a < 0.1) {
        discard;
    }

    FragColor = textureColor;
}
