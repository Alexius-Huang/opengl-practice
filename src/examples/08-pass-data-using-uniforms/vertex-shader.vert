#version 330 core
// Passing multiple attributes through different locations
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

// Passing data through uniform
uniform float uTime;

// Declare to pass data out of this shader
out vec4 vColor;

// Rotation Matrix
mat3 rotationZ(float angle) {
    float s = sin(angle);
    float c = cos(angle);

    return mat3(
         c, -s, 0.0,
         s,  c, 0.0,
         0.0, 0.0, 1.0
    );
}

void main() {
    gl_Position = vec4(aPos * rotationZ(uTime), 1.0);

    // assign the data to pass out of this shader
    vColor = vec4(aColor, 1.0);
}
