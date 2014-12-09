#version 330 core

in vec2 uv;

uniform samplerCube textureSampler;

out vec4 fragColor;

void main () {
  vec4 texColor = texture2D(textureSampler, uv);
  fragColor = texColor;
}
