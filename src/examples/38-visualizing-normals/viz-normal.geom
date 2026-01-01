#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in vec3 vNormal[];
uniform mat4 uProjection;

void main() {
    for (int i = 0; i < 3; i++) {
        gl_Position = uProjection * (gl_in[i].gl_Position);
        EmitVertex();

        gl_Position = uProjection * (gl_in[i].gl_Position + vec4(vNormal[i], .0) * .2);
        EmitVertex();
        EndPrimitive();
    }
}
