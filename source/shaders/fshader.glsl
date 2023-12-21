// Fragment Shader
#version 330 core

in vec3 fragNormal;
in vec3 fragPosition;
out vec4 color;

uniform vec3 camPos;
uniform vec4 lsPos;  // Light source position
uniform vec4 lsColor;  // Light source color
uniform int alpha;  // Shininess coefficient

vec4 Ia = vec4(0.1, 0.1, 0.1, 1.0);  // Ambient intensity
vec4 ka = vec4(0.9551, 0.9551, 0.9551, 1.0);  // Ambient reflectivity
vec4 kd = vec4(0.6163, 0.6163, 0.6163, 1.0);  // Diffuse reflectivity
vec4 ks = vec4(0.3000, 0.3000, 0.3000, 1.0);  // Specular reflectivity

void main() {
    vec3 normal = normalize(fragNormal);
    vec3 lightDir = normalize(lsPos.xyz - fragPosition);
    vec3 viewDir = normalize(camPos - fragPosition);

    // Ambient component
    vec4 ambient = Ia * ka;

    // Diffuse component
    float diff = max(dot(normal, lightDir), 0.0);
    vec4 diffuse = lsColor * kd * diff;

    // Specular component
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), alpha);
    vec4 specular = lsColor * ks * spec;

    color = ambient + diffuse + specular;
}
