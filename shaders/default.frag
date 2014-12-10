#version 330 core
uniform float k_a, k_d;
uniform vec3 object_a, object_d, ambient_intensity;

in vec3 pos;
in vec3 nnorm;
in vec4 pos4;

const vec3 light = vec3(3.0f, 0.0f, 0.0f);

void main()
{
  vec3 norm = -nnorm;
  vec3 lightDirection = normalize(light - pos);
  //lightDirection.y = -lightDirection.y;
  float d = max(0.0f, dot(norm, lightDirection));
  float specular = 0.0f;
  /*if(d > 0.0f) {
    vec3 reflected = reflect(-lightDirection, norm);
    float specularAngle = max(0.0f, dot(reflected, normalize(pos)));
    specular = pow(specularAngle, 5.0f);
  }*/
  vec3 color = ambient_intensity*object_a*k_a +
               object_d*d*k_d + vec3(specular);
  clamp(color, 0.0f, 1.0f);
  gl_FragColor = vec4(color, 1.0f);
}
