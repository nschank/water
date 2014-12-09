#version 330 core
uniform float k_a, k_d;
uniform vec3 w_a, w_d, ambient_intensity;

varying vec3 pos;
varying vec3 norm;
varying vec4 pos4;

const vec3 light = vec3(0.0f, 9.0f, 0.0f);

void main()
{
  /*vec3 vPos = vec3(pos4)/pos4.w;
  vec3 lightDirection = normalize(light - pos);
  lightDirection.y = -lightDirection.y;
  float d = max(0.0f, dot(norm, lightDirection));
  float specular = 0.0f;
  if(d > 0.0f) {
    //lightDirection = normalize(light - vPos);
    //lightDirection.y = -lightDirection.y;
    vec3 reflected = reflect(-lightDirection, norm);
    // either use normalize(vPos) or normalize(pos)                   ???
    float specularAngle = max(0.0f, dot(reflected, normalize(vPos)));
    specular = pow(specularAngle, 5.0f);
  }
  vec3 color = ambient_intensity*w_a*k_a + w_d*d*k_d + vec3(specular, specular, specular);
  clamp(color, 0.0f, 1.0f);
  gl_FragColor = vec4(color, 1.0f);*/
  gl_FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);
}
