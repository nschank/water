#version 330 core

uniform float k_a, k_d;
uniform vec3 water_a, water_d, ambient_intensity;

uniform vec3 object_a, object_d;

uniform samplerCube cubeMap;
uniform vec3 cameraPosition;

uniform mat3 normal_matrix;

uniform mat3 ballNormalMatrices0;
uniform mat4 ballModels0;
uniform mat3 ballNormalMatrices1;
uniform mat4 ballModels1;
uniform mat3 ballNormalMatrices2;
uniform mat4 ballModels2;

in vec3 pos;
in vec3 norm;

// roughly where the sun is in the skybox
const vec3 light = vec3(.5f, 1.f, .4f);

void main() {
  vec3 lightDirection = normalize(light - pos);
  float dotProduct = max(0.0f, dot(norm, lightDirection));
  float specular = 0.0f;
  if(dotProduct > 0.0f) {
    vec3 reflected = reflect(-lightDirection, norm);
    float specularAngle = max(0.0f, dot(reflected, normalize(pos)));
    specular = pow(specularAngle, 5.0f); // using a specular exponent of 5
  }

  vec3 toCamera = normalize(pos - cameraPosition);
  vec3 reflectionDir = reflect(toCamera, norm);

  // test for intersection with any of the balls
  float ballRadius = 0.5f;
  vec3 sphereReflectionColor = vec3(0.0f);
  float intersection0 = -1.0f;
  vec3 ballNormal0 = vec3(0.0f);
  mat4 ballInvModel0 = inverse(ballModels0);
  vec4 P0 = ballInvModel0*vec4(pos, 1.0f);
  vec4 d0 = ballInvModel0*vec4(reflectionDir, 0.0f);
  float a0 = d0.x*d0.x + d0.y*d0.y + d0.z*d0.z,
        b0 = 2.0f*(P0.x*d0.x + P0.y*d0.y + P0.z*d0.z),
        c0 = P0.x*P0.x + P0.y*P0.y + P0.z*P0.z - 0.25f;
  float disc0 = b0*b0 - 4.0f*a0*c0;
  if(disc0 >= 0.0f) {
    float first = (-b0 + sqrt(disc0))/(2.0f*a0),
          second = (-b0 - sqrt(disc0))/(2.0f*a0);
    float t = -1.0f;
    bool firstNonNeg = false,
         secondNonNeg = false;
    if(first >= 0.0f) {
      firstNonNeg = true;
    }
    if(second >= 0.0f) {
      secondNonNeg = true;
    }
    if(firstNonNeg && first <= second) {
      t = first;
      vec4 objectPoint = P0 + t*d0;
      ballNormal0 = normalize(ballNormalMatrices0*vec3(objectPoint));
    } else if(secondNonNeg) {
      t = second;
      vec4 objectPoint = P0 + t*d0;
      ballNormal0 = normalize(ballNormalMatrices0*vec3(objectPoint));
    }
    intersection0 = t;
  }
  
  float intersection1 = -1.0f;
  vec3 ballNormal1 = vec3(0.0f);
  mat4 ballInvModel1 = inverse(ballModels1);
  vec4 P1 = ballInvModel1*vec4(pos, 1.0f);
  vec4 d1 = ballInvModel1*vec4(reflectionDir, 0.0f);
  float a1 = d1.x*d1.x + d1.y*d1.y + d1.z*d1.z,
        b1 = 2.0f*(P1.x*d1.x + P1.y*d1.y + P1.z*d1.z),
        c1 = P1.x*P1.x + P1.y*P1.y + P1.z*P1.z - 0.25f;
  float disc1 = b1*b1 - 4.0f*a1*c1;
  if(disc1 >= 0.0f) {
    float first = (-b1 + sqrt(disc1))/(2.0f*a1),
          second = (-b1 - sqrt(disc1))/(2.0f*a1);
    float t = -1.0f;
    bool firstNonNeg = false,
         secondNonNeg = false;
    if(first >= 0.0f) {
      firstNonNeg = true;
    }
    if(second >= 0.0f) {
      secondNonNeg = true;
    }
    if(firstNonNeg && first <= second) {
      t = first;
      vec4 objectPoint = P1 + t*d1;
      ballNormal1 = normalize(ballNormalMatrices1*vec3(objectPoint));
    } else if(secondNonNeg) {
      t = second;
      vec4 objectPoint = P1 + t*d1;
      ballNormal1 = normalize(ballNormalMatrices1*vec3(objectPoint));
    }
    intersection1 = t;
  }
  
  float intersection2 = -1.0f;
  vec3 ballNormal2 = vec3(0.0f);
  mat4 ballInvModel2 = inverse(ballModels2);
  vec4 P2 = ballInvModel2*vec4(pos, 2.0f);
  vec4 d2 = ballInvModel2*vec4(reflectionDir, 0.0f);
  float a2 = d2.x*d2.x + d2.y*d2.y + d2.z*d2.z,
        b2 = 2.0f*(P2.x*d2.x + P2.y*d2.y + P2.z*d2.z),
        c2 = P2.x*P2.x + P2.y*P2.y + P2.z*P2.z - 0.25f;
  float disc2 = b2*b2 - 4.0f*a2*c2;
  if(disc2 >= 0.0f) {
    float first = (-b2 + sqrt(disc2))/(2.0f*a2),
          second = (-b2 - sqrt(disc2))/(2.0f*a2);
    float t = -1.0f;
    bool firstNonNeg = false,
         secondNonNeg = false;
    if(first >= 0.0f) {
      firstNonNeg = true;
    }
    if(second >= 0.0f) {
      secondNonNeg = true;
    }
    if(firstNonNeg && first <= second) {
      t = first;
      vec4 objectPoint = P2 + t*d2;
      ballNormal2 = normalize(ballNormalMatrices2*vec3(objectPoint));
    } else if(secondNonNeg) {
      t = second;
      vec4 objectPoint = P2 + t*d2;
      ballNormal2 = normalize(ballNormalMatrices2*vec3(objectPoint));
    }
    intersection2 = t;
  }

  if((intersection0 < intersection1 && intersection0 > 0.0f) || intersection1 < 0.0f) {
    vec3 spherePos = pos + intersection0*reflectionDir;
    float d = max(0.0f, dot(ballNormal0, normalize(light - spherePos)));
    sphereReflectionColor = object_d*d;
  } else if((intersection1 < intersection0 && intersection1 > 0.0f) || intersection0 < 0.0f) {
    vec3 spherePos = pos + intersection1*reflectionDir;
    float d = max(0.0f, dot(ballNormal1, normalize(light - spherePos)));
    sphereReflectionColor = object_d*d;
  }


  float F = 0.0f;
  vec3 color = (1-F)*(//ambient_intensity*water_a*k_a +
                      /*water_d*dotProduct*k_d + vec3(specular) + */sphereReflectionColor) +
               vec3(F*texture(cubeMap, normalize(reflectionDir+pos+vec3(0,.12,0))));
  clamp(color, 0.0f, 1.0f);
  gl_FragColor = vec4(color, 0.0f);
}
