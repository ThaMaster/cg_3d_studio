#version 330 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 aTexCoord;
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
