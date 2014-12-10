#version 330 core

uniform float k_a, k_d;
uniform vec3 object_a, object_d, ambient_intensity;

uniform samplerCube cubeMap;

in vec3 pos;
in vec3 norm;
in mat4 view;
in vec3 vertexToEye;

const vec3 light = vec3(-3.0f, -3.0f, -3.0f);

void main()
{
  /*float n1 = 1.0f;
  float n2 = 1.5f;
  float eta = n2/n1;
  float r0 = ((n1-n2)/(n1+n2))*((n1-n2)/(n1+n2));
  float m = 0.8f;
  vec3 lightDirection = normalize(light - pos);
  float d = max(0.0f, dot(norm, lightDirection));
  float specular = 0.0f;
  if(d > 0.0f) {
    vec3 reflected = reflect(-lightDirection, norm);
    float specularAngle = max(0.0f, dot(reflected, normalize(pos)));
    specular = pow(specularAngle, 5.0f);
  }
  vec3 eyeToVertex = normalize(pos); //remember we are in eye space!
  vec3 h = normalize(vertexToEye+light);
  vec3 e = normalize(vertexToEye);

  float alpha = acos(dot(norm, h));
  float D = exp(-1*pow(tan(alpha)/m, 2))/(4*m*m*pow(dot(norm, h), 4));
  //float F = r0 + (1-r0)*pow(1-dot(h, e), 5);
  float F = 0.0f;
  float G = min(2*dot(h, norm)*dot(e, norm)/dot(e, h), 2*dot(h, norm)*dot(lightDirection, norm)/dot(e, h));
  G = min(G, 1);
  float ks = 1;//clamp(D*F*G/dot(e, norm), 0, 1);

  vec3 worldNormal = normalize(mat3(transpose(view))*norm);
  vec3 worldLook = normalize(mat3(transpose(view))*eyeToVertex);
  vec3 incident = reflect(worldLook, worldNormal);

  vec3 color = (1-F)*(ambient_intensity*object_a*k_a +
               object_d*d*k_d + vec3(ks*specular));// + +
               //refract(worldLook, worldNormal, eta));// +
               //vec3(F*texture(cubeMap, incident));
  clamp(color, 0.0f, 1.0f);
  gl_FragColor = vec4(color, 1.0f);*/
  vec3 lightDirection = normalize(light - pos);
  //lightDirection.y = -lightDirection.y;
  float d = max(0.0f, dot(norm, lightDirection));
  vec3 color = ambient_intensity*object_a*k_a +
               object_d*d*k_d;
  clamp(color, 0.0f, 1.0f);
  gl_FragColor = vec4(color, 1.0f);
}
