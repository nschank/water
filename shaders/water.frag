#version 330 core
uniform float k_a, k_d;
uniform vec3 w_a, w_d, ambient_intensity;

varying vec3 pos;
varying vec3 norm;
varying vec4 pos4;

const vec3 light = vec3(0.0f, 9.0f, 0.0f);

void main()
{
  gl_FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);
}
