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

vec4 ambient;
vec4 diffuse;
vec4 specular;

vec4 Ia = vec4( 0.1, 0.1, 0.1, 1.0 );
vec4 ka = vec4( 1.0, 1.0, 1.0, 1.0 );
vec4 kd = ka;
vec4 ks = kd;

void
main()
{
    mat3 normalMatrix = transpose(inverse(mat3(M)));
    vec3 normal = normalize(normalMatrix * vNormal);

    vec4 viewPosition = V*M*vec4(vPosition, 1.0);
    vec4 viewSunPos = V*lsPos;
    vec4 l = normalize(viewSunPos - viewPosition);
    vec4 viewCamPos = V * vec4(camPos, 1.0);
    vec4 v = normalize(viewCamPos - viewPosition);
    ambient = Ia * ka;

    float diffIntensity = max(dot(normal, l.xyz), 0);
    
    if(diffIntensity > 0.0) {

        diffuse = lsColor * kd * diffIntensity;        
        vec4 r = vec4(reflect( l.xyz, normal), 1.0);
        float specIntensity = pow( max( dot( v, r ), 0 ),  alpha);
        specular = lsColor * ks * specIntensity;

        color = ambient + diffuse + specular;

    } else {

        color = ambient;

    }
        
    gl_Position =  P * V * M * vec4( vPosition, 1.0 );
}

