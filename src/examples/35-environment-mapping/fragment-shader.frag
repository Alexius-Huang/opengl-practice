#version 330 core
out vec4 FragColor;

uniform sampler2D uTexture;
uniform samplerCube uSkyboxTexture;
uniform vec3 uCameraPosition;
uniform int uMode;

in vec2 vTexCoords;
in vec3 vNormal;
in vec3 vFragmentPosition;

void main() {
    // Incident vector describes the direction where camera is looking at the fragment
    vec3 incidentVector = normalize(vFragmentPosition - uCameraPosition);

    if (uMode == 1) {
        vec3 reflection = reflect(incidentVector, normalize(vNormal));
        FragColor = vec4(vec3(texture(uSkyboxTexture, reflection)), 1.0);
    } else if (uMode == 2) {
        // Simulation of glass's refraction
        // However, if you want to simulate physically, we have to perform one more refraction
        // when the light goes outside of the material, this is only one-sided refraction
        float refractionRatio = 1.0 / 1.52;
        vec3 refraction = refract(incidentVector, normalize(vNormal), refractionRatio);
        FragColor = vec4(vec3(texture(uSkyboxTexture, refraction)), 1.0);
    }
}
