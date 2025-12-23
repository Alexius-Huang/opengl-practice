#version 330 core
out vec4 FragColor;

uniform vec3 uViewPosition;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material uMaterial;

struct SpotLight {
    // Spot light has positions and pointed direction with cutoff angle
    vec3 position;
    vec3 direction;
    float cutoff;
    float outerCutoff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform SpotLight uLight;

in vec3 vNormal;
in vec3 vFragmentPosition;

// We need the texture coordinate for using light map
in vec2 vTexCoords;

void main() {
    vec3 materialDiffuseColor = vec3(texture(uMaterial.diffuse, vTexCoords));
    vec3 materialSpecularColor = vec3(texture(uMaterial.specular, vTexCoords));

    // Direction to the light source from fragment
    vec3 _lightDirection = uLight.position - vFragmentPosition;
    vec3 lightDirection = normalize(_lightDirection);

    // Deriving ambient component
    vec3 ambient = uLight.ambient * materialDiffuseColor;

    // We calculate the cosine value 
    float cosineLight = dot(lightDirection, -normalize(uLight.direction));

    // Out of spotlight range, we only use ambient color
    if (uLight.outerCutoff > cosineLight) {
        FragColor = vec4(ambient, 1.0);
        return;
    }

    // outer to inner cutoff ratio to represent the transition of the light
    float fadeInRatio = min(
        (cosineLight - uLight.outerCutoff) / (uLight.cutoff - uLight.outerCutoff),
        1.0
    );

    // Or using clamp:
    // float fadeInRatio = clamp(
    //     (cosineLight - uLight.outerCutoff) / (uLight.cutoff - uLight.outerCutoff),
    //     0.0,
    //     1.0
    // );

    // Deriving diffuse component
    vec3 norm = normalize(vNormal);
    vec3 diffuse = uLight.diffuse * max(dot(norm, lightDirection), 0.0) * materialDiffuseColor;

    // Derive specular component
    vec3 viewDirection = normalize(uViewPosition - vFragmentPosition);
    vec3 reflected = reflect(-lightDirection, norm);
    float spec = pow(max(dot(reflected, viewDirection), 0.0), uMaterial.shininess);
    vec3 specular = uLight.specular * spec * materialSpecularColor;

    // Phong Lighting System is the sum of ambient, diffuse and specular
    vec3 objectColor = ambient + (diffuse + specular) * fadeInRatio;
    FragColor = vec4(objectColor, 1.0);
}
