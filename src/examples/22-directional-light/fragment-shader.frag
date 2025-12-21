#version 330 core
out vec4 FragColor;

uniform vec3 uViewPosition;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material uMaterial;

// For light source, it also has its own intensity on different component
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light uLight;

in vec3 vNormal;
in vec3 vFragmentPosition;

// We need the texture coordinate for using light map
in vec2 vTexCoords;

void main() {
    vec3 materialDiffuseColor = vec3(texture(uMaterial.diffuse, vTexCoords));
    vec3 materialSpecularColor = vec3(texture(uMaterial.specular, vTexCoords));

    // Deriving ambient component
    vec3 ambient = uLight.ambient * materialDiffuseColor;

    // Deriving diffuse component
    vec3 norm = normalize(vNormal);
    vec3 lightDirection = normalize(uLight.position - vFragmentPosition);
    vec3 diffuse = uLight.diffuse * max(dot(norm, lightDirection), 0.0) * materialDiffuseColor;

    // Derive specular component
    vec3 viewDirection = normalize(uViewPosition - vFragmentPosition);
    vec3 reflected = reflect(-lightDirection, norm);
    float spec = pow(max(dot(reflected, viewDirection), 0.0), uMaterial.shininess);
    vec3 specular = uLight.specular * spec * materialSpecularColor;

    // Phong Lighting System is the sum of ambient, diffuse and specular
    vec3 objectColor = ambient + diffuse + specular;
    FragColor = vec4(objectColor, 1.0);
}
