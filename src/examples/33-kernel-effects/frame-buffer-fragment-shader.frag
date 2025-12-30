#version 330 core
out vec4 FragColor;
  
in vec2 vTexCoords;

uniform sampler2D uScreenTexture;
uniform float uTexCoordOffset;
uniform int uMode;

const float kernelMatrices[36] = float[](
    0.0, 0.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 0.0, 0.0,

    -1.0, -1.0, -1.0,
    -1.0,  9.0, -1.0,
    -1.0, -1.0, -1.0,

    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16,

    1.0,  1.0, 1.0,
    1.0, -8.0, 1.0,
    1.0,  1.0, 1.0
);

void main() { 
    vec2 offsets[9] = vec2[](
        vec2(-uTexCoordOffset,  uTexCoordOffset), // top-left
        vec2(            0.0f,  uTexCoordOffset), // top-center
        vec2( uTexCoordOffset,  uTexCoordOffset), // top-right
        vec2(-uTexCoordOffset,             0.0f), // center-left
        vec2(            0.0f,             0.0f), // center-center
        vec2( uTexCoordOffset,             0.0f), // center-right
        vec2(-uTexCoordOffset, -uTexCoordOffset), // bottom-left
        vec2(            0.0f, -uTexCoordOffset), // bottom-center
        vec2( uTexCoordOffset, -uTexCoordOffset)  // bottom-right    
    );

    vec3 color = vec3(0.0);

    for (int i = 0; i < 9; i++) {
        vec3 textureColor = vec3(texture(uScreenTexture, vTexCoords + offsets[i]));

        int kernelIndex = (uMode - 1) * 9 + i;
        float textureWeight = kernelMatrices[kernelIndex];
        color += textureColor * textureWeight;
    }

    FragColor = vec4(color, 1.0);
}
