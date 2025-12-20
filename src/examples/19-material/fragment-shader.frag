#version 330 core
out vec4 FragColor;

uniform vec3 uViewPosition;

// Declare material struct
// We can use http://devernay.free.fr/cours/opengl/materials.html to simulate real world material
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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

void main() {
    // Deriving ambient component
    vec3 ambient = uLight.ambient * uMaterial.ambient;

    // Deriving diffuse component
    vec3 norm = normalize(vNormal);
    vec3 lightDirection = normalize(uLight.position - vFragmentPosition);
    vec3 diffuse = uLight.diffuse * max(dot(norm, lightDirection), 0.0) * uMaterial.diffuse;

    // Derive specular component
    vec3 viewDirection = normalize(uViewPosition - vFragmentPosition);
    vec3 reflected = reflect(-lightDirection, norm);
    float spec = pow(max(dot(reflected, viewDirection), 0.0), uMaterial.shininess);
    vec3 specular = uLight.specular * spec * uMaterial.specular;

    // Phong Lighting System is the sum of ambient, diffuse and specular
    vec3 objectColor = ambient + diffuse + specular;
    FragColor = vec4(objectColor, 1.0);
}
