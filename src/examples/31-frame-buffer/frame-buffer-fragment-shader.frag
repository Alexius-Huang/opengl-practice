#version 330 core
out vec4 FragColor;
  
in vec2 vTexCoords;

uniform sampler2D uScreenTexture;

void main() { 
    FragColor = texture(uScreenTexture, vTexCoords);
}
