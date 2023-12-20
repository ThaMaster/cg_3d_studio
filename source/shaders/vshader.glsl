#version 330

in vec3 vNormal;
in vec3 vPosition;
out vec4 color;

uniform vec3 camPos;
uniform vec4 lsPos;
uniform vec4 lsColor;
uniform int alpha;
uniform mat4 M;
uniform mat4 P;
uniform mat4 V;

vec4 Ia = vec4( 0.1, 0.1, 0.1, 1.0 );

vec4 ka = vec4(0.9551, 0.9551, 0.9551, 1.0);
vec4 kd = vec4(0.6163, 0.6163, 0.6163, 1.0);
vec4 ks = vec4(0.3000, 0.3000, 0.3000, 1.0);

vec4 diffuse = vec4(0.0, 0.0, 0.0, 0.0);
vec4 specular = vec4(0.0, 0.0, 0.0, 0.0);

void
main()
{
    // Calculate the coordinates to world coordinates.
    vec3 normal = normalize(mat3(M) * vNormal);
    vec4 worldPosition = M * vec4(vPosition, 1.0);
    vec3 worldLightPos = lsPos.xyz; // Assuming lsPos is a point light in world space
    vec3 lightDir = normalize(worldLightPos - worldPosition.xyz);
    vec3 viewDir = normalize(camPos - worldPosition.xyz);
    viewDir = normalize(lsPos.xyz);

    // Calculate the three light components.
    vec4 ambient = Ia * ka;
    float diff = max(dot(normal, lightDir), 0);
    if(diff > 0.0) {
        diffuse = lsColor * kd * diff;

        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), alpha);
        specular = lsColor * ks * spec;
    }
    
    color = ambient + diffuse + specular;
        
    gl_Position =  P * V * M * vec4( vPosition, 1.0 );
}

