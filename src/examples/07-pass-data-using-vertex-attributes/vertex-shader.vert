#version 330 core
// Passing multiple attributes through different locations
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

// Declare to pass data out of this shader
out vec4 vColor;

void main() {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);

    // assign the data to pass out of this shader
    vColor = vec4(aColor, 1.0);
}
