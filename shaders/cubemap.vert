#version 330 core

in vec3 position;
in vec2 texCoord;

out vec2 uv;

uniform mat4 mvp;

void main () {
  uv = texCoord;
  gl_Position = mvp * vec4(position, 1.0);
}
