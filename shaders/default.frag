#version 330 core

uniform float k_a, k_d;
uniform vec3 O_a, O_d, i_a;

varying vec3 pos;
varying vec3 norm;
varying vec3 l;

void main()
{
  float d = max(0.0, dot(normalize(norm), normalize(light - pos)));
  vec3 color = i_a*O_a*k_a + O_d*d*k_d;
  gl_FragColor = vec4(color, 1.0);
}
