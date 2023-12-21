// Vertex Shader
#version 330 core

in vec3 vNormal;
in vec3 vPosition;
out vec3 fragNormal;
out vec3 fragPosition;

uniform mat4 M;
uniform mat4 P;
uniform mat4 V;

void main() {
    fragNormal = normalize(mat3(M) * vNormal);
    vec4 worldPosition = M * vec4(vPosition, 1.0);
    fragPosition = worldPosition.xyz;

    gl_Position = P * V * worldPosition;
}
