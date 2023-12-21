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

    if(hasTexCoords) {

        texCoord = aTexCoord;

    } else {
        // Assuming the mapping is a sphere, calculate the texture coords.

        vec3 normalizedPos = normalize(worldPosition).xyz;
        float s = acos(normalizedPos.x) / 3.14159265;
        float t = (atan(normalizedPos.z, normalizedPos.y) / 3.14159265) + 0.5;

        texCoord = vec2( s, t );
    }
    
    gl_Position = P * V * worldPosition;
}
