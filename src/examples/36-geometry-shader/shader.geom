
#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

// To pass data from vertex -> geometry -> fragment shader
// Geometry shader may contain multiple vertices, which means we will
// always need to declare in variable as array type
in vec3 vColor[];

out vec3 gColor;

const vec4 translation[5] = vec4[5](
    vec4(-.2, -.2, .0, .0), // bottom-left
    vec4( .2, -.2, .0, .0), // bottom-right
    vec4(-.2,  .2, .0, .0), // top-left
    vec4( .2,  .2, .0, .0), // top-right
    vec4( .0,  .4, .0, .0)  // top
);

void main() {
    gColor = vColor[0];
 
    for (int i = 0; i < 5; i++) {
        gl_Position = gl_in[0].gl_Position + translation[i]; 

        // we can also modify and pass different color to the fragment shader
        if (i == 4) {
            gColor = vec3(1.0, 1.0, 1.0);
        }
        EmitVertex();
    }

    EndPrimitive();
}  
