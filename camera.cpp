#include "camera.h"

Camera::Camera(int px_w, int px_h)
{
  m_px_w = px_w;
  m_px_h = px_h;
	setClip(NEAR_PLANE, FAR_PLANE);
  setHeightAngle(HEIGHT_ANGLE);
  setAspectRatio(1.0f);
	if(LOOK_SETTING_ACROSS)
		orientLook(glm::vec4(0.f, .15f, 1.0f, 1.0f),
				 glm::vec4(0.0f, 0.0f, -1.0f, 0),
				 glm::vec4(0.0f, 1.0f, 0.0f, 0));
	else
		orientLook(glm::vec4(0.0f, .5f, 0.0f, 1.0f),
				   glm::vec4(0.0f, -1.0f, 0.0f, 0),
				   glm::vec4(1.0f, 0.0f, 0.0f, 0));
}

void Camera::setAspectRatio(float a)
{
  aspectRatio = a;
  calculateScale();
}

glm::mat4x4 Camera::getProjectionMatrix() const
{
  return projectionMatrix;
}

// M3*M4?
void Camera::calculateView() {
viewMatrix = glm::transpose(
                  glm::mat4x4(1.0f, 0.0f, 0.0f, -eye.x,
                              0.0f, 1.0f, 0.0f, -eye.y,
                              0.0f, 0.0f, 1.0f, -eye.z,
                              0.0f, 0.0f, 0.0f, 1.0f)*
                  glm::mat4x4(u.x, u.y, u.z, 0.0f,
                              v.x, v.y, v.z, 0.0f,
                              w.x, w.y, w.z, 0.0f,
                              0.0f, 0.0f, 0.0f, 1.0f));
}

// M2?
void Camera::calculateScale() {
  float tanHalfHeightAngle = glm::tan(heightAngle/2.0f);
  float tanHalfWidthAngle = aspectRatio*tanHalfHeightAngle;
  scaleMatrix = glm::transpose(
                glm::mat4x4(1.0f/(farPlane*tanHalfWidthAngle), 0.0f, 0.0f, 0.0f,
                            0.0f, 1.0f/(farPlane*tanHalfHeightAngle), 0.0f, 0.0f,
                            0.0f, 0.0f, 1.0f/farPlane, 0.0f,
                            0.0f, 0.0f, 0.0f, 1.0f));
  projectionMatrix = perspectiveMatrix*scaleMatrix;
}

float Camera::getAspectRatio() {
  return aspectRatio;
}

glm::mat4x4 Camera::getViewMatrix() const
{
  return viewMatrix;
}

glm::mat4x4 Camera::getScaleMatrix() const
{
  return scaleMatrix;
}

glm::mat4x4 Camera::getPerspectiveMatrix() const
{
  return perspectiveMatrix;
}

glm::vec4 Camera::getPosition() const
{
  return eye;
}

glm::vec4 Camera::getLook() const
{
  return glm::vec4(-w, 0);
}

glm::vec4 Camera::getUp() const
{
  return glm::vec4(v, 0);
}

float Camera::getAspectRatio() const
{
  return aspectRatio;
}

float Camera::getHeightAngle() const
{
  return heightAngle;
}

void Camera::orientLook(const glm::vec4 &eyeVec,
                                const glm::vec4 &lookVec,
                                const glm::vec4 &upVec)
{
  eye = eyeVec;
  look = lookVec;
  up = upVec;
  glm::vec3 up3(up);
  glm::vec3 look3(look);
  w = -glm::normalize(look3);
  v = glm::normalize((up3 - glm::dot(up3, w)*w));
  u = glm::normalize(glm::cross(v, w));
  calculateView();
}

void Camera::setHeightAngle(float h)
{
  heightAngle = glm::radians(h);
  calculateScale();
}

void Camera::translate(const glm::vec4 &v)
{
  eye += v;
  calculateView();
}

void Camera::rotateU(float degrees)
{
  glm::vec3 tu, tv, tw;
  tu = u;
  tv = v;
  tw = w;
  float radians = glm::radians(degrees);
  v = glm::normalize(tv*glm::cos(radians) + tw*glm::sin(radians));
  w = glm::normalize(tw*glm::cos(radians) - tv*glm::sin(radians));
  calculateView();
}


void Camera::rotateV(float degrees)
{
  glm::vec3 tu, tv, tw;
  tu = u;
  tv = v;
  tw = w;
  float radians = glm::radians(degrees);
  u = glm::normalize(tu*glm::cos(radians) - tw*glm::sin(radians));
  w = glm::normalize(tu*glm::sin(radians) + tw*glm::cos(radians));
  calculateView();
}

void Camera::rotateW(float degrees)
{
  glm::vec3 tu, tv, tw;
  tu = u;
  tv = v;
  tw = w;
  float radians = glm::radians(degrees);
  u = glm::normalize(tu*glm::cos(radians) + tv*glm::sin(radians));
  v = glm::normalize(tv*glm::cos(radians) - tu*glm::sin(radians));
  calculateView();
}

void Camera::setClip(float nearPlane, float farPlane)
{
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;
	float c = -nearPlane/farPlane;
    perspectiveMatrix = glm::perspective(55.0f, 1.0f, 0.01f, 10.0f);
    calculateScale();
}

glm::vec4 Camera::getEye() {
  return eye;
}

bool Camera::CastRayAtObject(glm::vec3 *hit, glm::mat4x4 model) {
	glm::vec3 model_dir = glm::mat3(glm::transpose(glm::inverse(model)))*-1.f*w;
	glm::vec4 model_eye = glm::inverse(model)*eye;

	float t = - model_eye.y/model_dir.y;

	*hit = model_dir*t + glm::vec3(model_eye);

  bool on_face = (hit->x >= -0.5 && hit->x <= 0.5 &&
                  hit->z >= -0.5 && hit->z <= 0.5);
  return (t >= 0 && on_face);
}


void Camera::MouseMoved(int dx, int dy) {
  // compute the new look vector based upon the old one
  float x_ratio = dx/float(m_px_w/2.0);      float y_ratio = dy/float(m_px_h/2.0);
  float real_width = nearPlane*tan((heightAngle*aspectRatio)/2.0);  float real_height = nearPlane*tan(heightAngle/2.0);
  float new_width = real_width * x_ratio;    float new_height = real_height * y_ratio;
  float x_angle = atan(new_width/nearPlane);    float y_angle = atan(new_height/nearPlane);

  float factor = 0.2f;


  rotateV(glm::degrees(-x_angle * factor));
  rotateU(glm::degrees(-y_angle * factor));
  orientLook(eye, -glm::vec4(w, 0.0), up);
}

void Camera::MouseScrolled(int units) {
	//glm::mat4x4 transform = glm::translate(-factor * float(units) * look);
	float f = SCROLL_TICK;
	f *= -1;
	f *= float(units);

	translate(f * glm::vec4(w,0));
}
