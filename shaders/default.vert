#version 330 core

in vec3 position;
in vec3 normal;

uniform mat4 p;
uniform mat4 v;
uniform mat4 m;

const vec3 light = vec3(0.0f, 3.0f, 3.0f);

varying vec3 pos;
varying vec3 norm;
varying vec3 l;

void main()
{
  mat4 MVP = p * v * m;
  gl_Position = MVP * vec4(position, 1.0f);
  pos = vec3(gl_Position);
  norm = vec3(MVP * vec4(norm, 1.0f));
  l = vec3(MVP * vec4(light, 1.0f));
}
