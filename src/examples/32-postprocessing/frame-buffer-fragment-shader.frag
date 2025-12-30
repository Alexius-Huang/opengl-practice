#version 330 core
out vec4 FragColor;
  
in vec2 vTexCoords;

uniform sampler2D uScreenTexture;
uniform int uMode;

void main() { 
    vec4 color = texture(uScreenTexture, vTexCoords);
    if (uMode == 1) {
        FragColor = color;
    } else if (uMode == 2) {
        FragColor = vec4(1.0 - vec3(color), 1.0);
    } else if (uMode == 3) {
        // Using so-called Luminosity Method
        vec3 luminosity = vec3(0.2126, 0.7152, 0.0722) * vec3(color);
        float grayscale = luminosity.r + luminosity.g + luminosity.b;
        FragColor = vec4(vec3(grayscale), 1.0);
    }
}
