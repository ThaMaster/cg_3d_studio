#version 330 core

in vec3 vNormal;
in vec3 vPosition;
in vec2 aTexCoord;
out vec3 fragNormal;
out vec3 fragPosition;
out vec2 texCoord;

uniform mat4 M;
uniform mat4 P;
uniform mat4 V;
uniform bool hasTexCoords;

void main() {
    fragNormal = normalize(mat3(M) * vNormal);
    vec4 worldPosition = M * vec4(vPosition, 1.0);
    fragPosition = worldPosition.xyz;

    texCoord = aTexCoord;
    
    gl_Position = P * V * worldPosition;
}
