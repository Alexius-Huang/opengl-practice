#version 330 core
layout (location = 0) in vec3 aPos;

// Declare to pass data out of this shader
out vec4 vColor;

void main() {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);

    // assign the data to pass out of this shader
    vColor = vec4(abs(aPos), 1.0);
}
