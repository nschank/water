#include "camera.h"

Camera::Camera(int px_w, int px_h)
{
    m_px_w = px_w;
    m_px_h = px_h;

    m_eye = glm::vec3(0.0f, 2.0f, 0.0f);
    m_target = glm::vec3(0.0f, 0.0f, 0.0f);
    m_up = glm::vec3(0.0f, 0.0f, -1.0f);

    m_fov = 55.0f;
    m_aspect = 1.0f;
    m_near = 0.01f;
    m_far = 10.0f;

    m_p = glm::perspective(m_fov, m_aspect, m_near, m_far);

    m_v = glm::lookAt(m_eye, m_target, m_up);
}

glm::mat4x4 Camera::P() {
    return m_p;
}

glm::mat4x4 Camera::V() {
    return m_v;
}

bool Camera::CastRayAtObject(glm::vec3 *hit, glm::mat4x4 model) {
    glm::vec3 look = glm::normalize(m_target - m_eye);
    glm::mat4x4 inv = glm::inverse(model);
    glm::vec3 model_dir = glm::vec3(inv*glm::vec4(look, 0.0));
    glm::vec3 model_eye = glm::vec3(inv*glm::vec4(m_eye, 1.0));

    float t = (0.5 - model_eye.y)/model_dir.y;

    *hit = model_dir*t + model_eye;

    bool on_face = (hit->x >= -0.5 && hit->x <= 0.5 &&
                    hit->z >= -0.5 && hit->z <= 0.5);
    return (t >= 0 && on_face);
}

void Camera::MouseMoved(int dx, int dy) {
    // compute the new look vector based upon the old one
    float x_ratio = dx/float(m_px_w/2.0);      float y_ratio = dy/float(m_px_h/2.0);
    float real_width = m_near*tan(m_fov/2.0);  float real_height = m_near*tan((m_fov*m_aspect)/2.0);
    float new_width = real_width * x_ratio;    float new_height = real_height * y_ratio;
    float x_angle = atan(new_width/m_near);    float y_angle = atan(new_height/m_near);

    float factor = 12.0f;

    glm::vec3 look = glm::normalize(m_target - m_eye);

    glm::mat4x4 transform = glm::translate(m_eye)*
                            glm::rotate(factor * x_angle, m_up) *
                            glm::rotate(factor * y_angle, glm::normalize(glm::cross(look, m_up))) *
                            glm::translate(-m_eye);
    m_eye = glm::vec3(transform * glm::vec4(m_eye, 1.0));
    m_target = glm::vec3(transform * glm::vec4(m_target, 1.0));
    m_up = glm::vec3(transform * glm::vec4(m_up, 1.0));

    m_v = glm::lookAt(m_eye, m_target, m_up);


}

void Camera::MouseScrolled(int units) {
    glm::vec3 look = glm::normalize(m_target - m_eye);
    float factor = 1.0/1200.0f;
    glm::mat4x4 transform = glm::translate(-factor * float(units) * look);
    m_eye = glm::vec3(transform * glm::vec4(m_eye, 1.0));
    m_target = glm::vec3(transform * glm::vec4(m_target, 1.0));
    m_up = glm::vec3(transform * glm::vec4(m_up, 1.0));

}
