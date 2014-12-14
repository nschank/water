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
  float specular = 0.0f;
  if(d > 0.0f) {
    vec3 reflected = reflect(-lightDirection, norm);
    float specularAngle = max(0.0f, dot(reflected, normalize(pos)));
    specular = pow(specularAngle, 5.0f);
  }
  vec3 eyeToVertex = normalize(pos); //remember we are in eye space!

  float F = 1.0f;

  vec3 worldNormal = normalize(mat3(transpose(view))*norm);
  vec3 worldLook = normalize(mat3(transpose(view))*eyeToVertex);
  vec3 incident = reflect(worldLook, worldNormal);

  vec3 color = (1-F)*(ambient_intensity*object_a*k_a +
               object_d*d*k_d + vec3(specular) +
               refract(worldLook, worldNormal, .75f)) +
               vec3(F*texture(cubeMap, incident));
  clamp(color, 0.0f, 1.0f);
  gl_FragColor = vec4(color, 1.0f);
}
