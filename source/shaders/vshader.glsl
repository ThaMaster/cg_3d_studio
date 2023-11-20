#version 330

in vec3 vPosition;

uniform mat4 M;
uniform mat4 P;
uniform mat4 V;

void
main()
{
    gl_Position =  P*V*M*vec4(vPosition, 1.0);
}
