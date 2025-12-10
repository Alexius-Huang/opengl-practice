#version 330 core
out vec4 FragColor;

// Declare the input of the data, the prefix v- immediately knows that
// it comes from our vertex shader
in vec4 vColor;

void main() {
    // We can simply use the input data which is from the output of the vertex shader
    FragColor = vColor;
}
