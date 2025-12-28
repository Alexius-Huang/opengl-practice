#version 330 core
out vec4 FragColor;

in vec2 vTexCoord;
uniform sampler2D uTexture;

uniform int uMode;

float near = 0.1;
float far = 100.0;

float linearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main() {
    // use texture function to render texture
    if (uMode == 1) {
        FragColor = texture(uTexture, vTexCoord);
        return;
    }

    // Visualizing depth buffer
    if (uMode == 2) {
        FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
        return;
    }

    // Visualizing for using the NDC with linearized depth
    float depth = linearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
    if (uMode == 3) {
        FragColor = vec4(vec3(depth), 1.0);
    }

    if (uMode == 4) {
        FragColor = vec4(mix(
            vec3(texture(uTexture, vTexCoord)),
            vec3(.0, .0, .0),
            depth
        ), 1.0);
    }
}
