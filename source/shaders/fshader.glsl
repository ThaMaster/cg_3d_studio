#version 330 core

in vec2 texCoord;
in vec3 fragNormal; // Normalized
in vec3 fragPosition; 

out vec4 color;

uniform vec3 camPos; // Camera Position
uniform vec4 la; // Ambient Light Intensity
uniform vec4 lsPos;  // Light source position
uniform vec4 lsColor;  // Light source color
uniform float alpha;  // Shininess coefficient

uniform sampler2D ourTexture;
uniform bool showTexture;

// Need to be able to map these later!
vec4 ka = vec4(1.0, 1.0, 1.0, 1.0); // Ambient reflectivity
vec4 kd = vec4(1.0, 1.0, 1.0, 1.0); // Diffuse reflectivity
vec4 ks = vec4(1.0, 1.0, 1.0, 1.0); // Specular reflectivity

void main() {
    vec3 lightDir = normalize(lsPos.xyz - fragPosition);
    vec3 viewDir = normalize(camPos - fragPosition);

    // Ambient component
    vec4 ambient = la * ka;

    // Diffuse component
    float diff = max(dot(fragNormal, lightDir), 0.0);
    vec4 diffuse = lsColor * kd * diff;

    // Specular component
    vec3 reflectDir = reflect(-lightDir, fragNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), alpha);
    vec4 specular = lsColor * ks * spec;
    
    if(showTexture) {
        vec4 textureColor = texture(ourTexture, texCoord);
        color = textureColor * (ambient + diffuse + specular);
    } else{
        color = ambient + diffuse + specular;
    }

}
