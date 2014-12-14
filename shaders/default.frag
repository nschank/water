#version 330 core

uniform float k_a, k_d;
uniform vec3 object_a, object_d, ambient_intensity;

uniform samplerCube cubeMap;

in vec3 pos;
in vec3 norm;
in mat4 view;
in vec3 vertexToEye;
in vec3 out_normal;
const vec3 light = vec3(.5f,1.f,.4f);

void main()
{
  vec3 lightDirection = normalize(light - pos);
  float d = max(0.0f, dot(norm, lightDirection));
  vec3 color = ambient_intensity*object_a*k_a +
               object_d*d*k_d;
  clamp(color, 0.0f, 1.0f);
  gl_FragColor = vec4(color, 1.0f);
}
