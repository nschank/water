#version 330 core

uniform float k_a, k_d;
uniform vec3 water_a, water_d, ambient_intensity;

uniform samplerCube cubeMap;
uniform vec3 cameraPosition;

in vec3 pos;
in vec3 norm;

// roughly where the sun is in the skybox
const vec3 light = vec3(.5f, 1.f, .4f);

void main() {
  vec3 lightDirection = normalize(light - pos);
  float d = max(0.0f, dot(norm, lightDirection));
  float specular = 0.0f;
  if(d > 0.0f) {
    vec3 reflected = reflect(-lightDirection, norm);
    float specularAngle = max(0.0f, dot(reflected, normalize(pos)));
    specular = pow(specularAngle, 8.0f);
  }
  float F = 0.95f;
  vec3 toCamera = normalize(pos - cameraPosition);
  vec3 reflectionDir = reflect(toCamera, norm);
  vec3 color = (1-F)*(ambient_intensity*water_a*k_a +
                      water_d*d*k_d + vec3(specular)) +
	       vec3(F*texture(cubeMap, reflectionDir-vec3(0,.12,0)));
  clamp(color, 0.0f, 1.0f);
  gl_FragColor = vec4(color, 0.0f);
}
