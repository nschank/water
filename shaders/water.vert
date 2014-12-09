#version 330 core

in vec3 position;
in vec3 normal;

uniform mat3 normal_matrix;
uniform mat4 p;
uniform mat4 v;
uniform mat4 m;

varying vec3 pos;
varying vec3 norm;
varying vec4 pos4;

void main()
{
  mat4 MVP = p * v * m;
  gl_Position = MVP * vec4(position, 1.0f);
  pos = vec3(m * vec4(position, 1.0f));
  pos4 = v * m * vec4(position, 1.0f);
  norm = normalize(normal_matrix * normal);
}
