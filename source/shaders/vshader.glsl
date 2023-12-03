#version 330

in vec3 vNormal;
in vec3 vPosition;

out vec4 color;

uniform mat4 M;
uniform mat4 P;
uniform mat4 V;

vec4 ambient;
vec4 diffuse;
vec4 specular;

vec4 Ia = vec4( 0.1, 0.1, 0.1, 1.0 );
vec4 Il = vec4( 0.4, 0.4, 0.2, 1.0 );
vec4 ka = vec4( 1.0, 1.0, 1.0, 1.0 );
vec4 kd = ka;
vec4 ks = kd;
vec4 l = normalize(vec4( 2.0, 0.5, 5.0, 0.0 ));
vec4 v = normalize(vec4( 0.0, 0.0, 2.0, 0.0 ));

void
main()
{
    ambient = Ia * ka;

    float diffIntensity = max(dot(normalize(vec4(vNormal, 1.0)), l), 0);
    
    if(diffIntensity > 0.0) {

        diffuse = Il * kd * diffIntensity;        
        vec4 r = normalize(reflect( -l, normalize(vec4( vNormal , 0.0 )) ));
        float specIntensity = pow( max( dot( v, r ), 0 ),  2);
        specular = Il * ks * specIntensity;

        color = ambient + diffuse + specular;

    } else {

        color = ambient;

    }
        
    gl_Position =  P * V * M * vec4( vPosition, 1.0 );
}
