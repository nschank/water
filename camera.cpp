#include "camera.h"
Camera::Camera(int px_w, int px_h)
{
    m_px_w = px_w;
    m_px_h = px_h;
    setAspectRatio(1.0);
    setClip(1.0, 30.0);
    setHeightAngle(30.0);
    orientLook(glm::vec4(2.0f, 2.0f, 2.0f, 0.0f),
               glm::vec4(-1.0f, -1.0f, -1.0f, 0.0f),
               glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));

}

void Camera::setAspectRatio(float a)
{
    m_aspect_ratio = a;

}

glm::mat4x4 Camera::getProjectionMatrix() const
{
    return m_proj_mat;
}

glm::mat4x4 Camera::getViewMatrix() const
{

    /*glm::mat4x4 pos_mat = glm::transpose(glm::mat4x4(1.0f, 0.0f, 0.0f, -m_eye.x,
                                                     0.0f, 1.0f, 0.0f, -m_eye.y,
                                                     0.0f, 0.0f, 1.0f, -m_eye.z,
                                                     0.0f, 0.0f, 0.0f, 1.0f));



    glm::mat4x4 look_mat = glm::transpose(glm::mat4x4(m_u.x, m_u.y, m_u.z, 0.0f,
                                                      m_v.x, m_v.y, m_v.z, 0.0f,
                                                      m_w.x, m_w.y, m_w.z, 0.0f,
                                                      0.0f, 0.0f, 0.0f, 1.0f));

    return look_mat * pos_mat;*/

    return m_view_mat;
}

glm::mat4x4 Camera::getScaleMatrix() const
{

    return m_scale_mat;
    /*return glm::transpose(glm::mat4x4(float(1.0/(m_far_plane*tan(m_height_angle/2.0)*m_aspect_ratio)), 0.0f, 0.0f, 0.0f,
                            0.0f, float(1.0/(m_far_plane*tan(m_height_angle/2.0))), 0.0f, 0.0f,
                            0.0f, 0.0f, float(1.0/m_far_plane), 0.0f,
                            0.0f, 0.0f, 0.0f, 1.0f));*/
}

glm::mat4x4 Camera::getM2() const {
    return m_scale_mat;
}

glm::mat4x4 Camera::getPerspectiveMatrix() const
{

    /*float c = -m_near_plane/m_far_plane;
    return glm::transpose(glm::mat4x4(1.0f, 0.0f, 0.0f, 0.0f,
                       0.0f, 1.0f, 0.0f, 0.0f,
                       0.0f, 0.0f, -1.0/(1 + c), c/(1 + c),
                       0.0f, 0.0f, -1.0f, 0.0f));*/
    return m_persp_mat;
}

glm::vec4 Camera::getPosition() const
{
    return m_eye;
}

glm::vec4 Camera::getLook() const
{
    return glm::vec4(-m_w, 0);
}

glm::vec4 Camera::getUp() const
{
    return glm::vec4(m_v, 0);
}

float Camera::getAspectRatio() const
{
    return m_aspect_ratio;
}

float Camera::getHeightAngle() const
{
    return m_height_angle;
}

glm::vec4 Camera::getEye() const {
    return m_eye;
}



void Camera::orientLook(const glm::vec4 &eye, const glm::vec4 &look, const glm::vec4 &up)
{

    m_eye = eye;
    m_look = look;
    m_up = up;


    m_w = -glm::vec3(m_look)/glm::length(glm::vec3(m_look));
    m_v = (glm::vec3(m_up)-(glm::dot(glm::vec3(m_up), m_w))*m_w)/glm::length(glm::vec3(m_up)-(glm::dot(glm::vec3(m_up), m_w))*m_w);
    m_u = glm::normalize(glm::cross(m_v, m_w));

    glm::mat4x4 pos_mat = glm::transpose(glm::mat4x4(1.0f, 0.0f, 0.0f, -m_eye.x,
                                                     0.0f, 1.0f, 0.0f, -m_eye.y,
                                                     0.0f, 0.0f, 1.0f, -m_eye.z,
                                                     0.0f, 0.0f, 0.0f, 1.0f));



    glm::mat4x4 look_mat = glm::transpose(glm::mat4x4(m_u.x, m_u.y, m_u.z, 0.0f,
                                                      m_v.x, m_v.y, m_v.z, 0.0f,
                                                      m_w.x, m_w.y, m_w.z, 0.0f,
                                                      0.0f, 0.0f, 0.0f, 1.0f));

    m_view_mat = look_mat * pos_mat;

    m_proj_mat = m_persp_mat * m_scale_mat;

}

void Camera::setHeightAngle(float h)
{
    m_height_angle = glm::radians(h);

    m_scale_mat = glm::transpose(glm::mat4x4(float(1.0/(m_far_plane*tan(m_height_angle/2.0)*m_aspect_ratio)), 0.0f, 0.0f, 0.0f,
                            0.0f, float(1.0/(m_far_plane*tan(m_height_angle/2.0))), 0.0f, 0.0f,
                            0.0f, 0.0f, float(1.0/m_far_plane), 0.0f,
                            0.0f, 0.0f, 0.0f, 1.0f));
    m_proj_mat = m_persp_mat * m_scale_mat;
}

void Camera::translate(const glm::vec4 &v)
{
    m_eye += v;
    glm::mat4x4 pos_mat = glm::transpose(glm::mat4x4(1.0f, 0.0f, 0.0f, -m_eye.x,
                                                     0.0f, 1.0f, 0.0f, -m_eye.y,
                                                     0.0f, 0.0f, 1.0f, -m_eye.z,
                                                     0.0f, 0.0f, 0.0f, 1.0f));



    glm::mat4x4 look_mat = glm::transpose(glm::mat4x4(m_u.x, m_u.y, m_u.z, 0.0f,
                                                      m_v.x, m_v.y, m_v.z, 0.0f,
                                                      m_w.x, m_w.y, m_w.z, 0.0f,
                                                      0.0f, 0.0f, 0.0f, 1.0f));

    m_view_mat = look_mat * pos_mat;
}

void Camera::rotateW(float degrees)
{

    glm::vec3 u, v, w;
    u = m_u;
    v = m_v;
    w = m_w;
    m_u = glm::normalize(v*glm::sin(glm::radians(degrees)) + u*glm::cos(glm::radians(degrees)));
    m_v = glm::normalize(v*glm::cos(glm::radians(degrees)) - u*glm::sin(glm::radians(degrees)));
    m_w = glm::normalize(w);
    glm::mat4x4 pos_mat = glm::transpose(glm::mat4x4(1.0f, 0.0f, 0.0f, -m_eye.x,
                                                     0.0f, 1.0f, 0.0f, -m_eye.y,
                                                     0.0f, 0.0f, 1.0f, -m_eye.z,
                                                     0.0f, 0.0f, 0.0f, 1.0f));



    glm::mat4x4 look_mat = glm::transpose(glm::mat4x4(m_u.x, m_u.y, m_u.z, 0.0f,
                                                      m_v.x, m_v.y, m_v.z, 0.0f,
                                                      m_w.x, m_w.y, m_w.z, 0.0f,
                                                      0.0f, 0.0f, 0.0f, 1.0f));

    m_view_mat = look_mat * pos_mat;
}

void Camera::rotateU(float degrees)
{
    glm::vec3 u, v, w;
    u = m_u;
    v = m_v;
    w = m_w;
    m_u = glm::normalize(u);
    m_v = glm::normalize(v*glm::cos(glm::radians(degrees)) + w*glm::sin(glm::radians(degrees)));
    m_w = glm::normalize(-v*glm::sin(glm::radians(degrees)) + w*glm::cos(glm::radians(degrees)));
    glm::mat4x4 pos_mat = glm::transpose(glm::mat4x4(1.0f, 0.0f, 0.0f, -m_eye.x,
                                                     0.0f, 1.0f, 0.0f, -m_eye.y,
                                                     0.0f, 0.0f, 1.0f, -m_eye.z,
                                                     0.0f, 0.0f, 0.0f, 1.0f));



    glm::mat4x4 look_mat = glm::transpose(glm::mat4x4(m_u.x, m_u.y, m_u.z, 0.0f,
                                                      m_v.x, m_v.y, m_v.z, 0.0f,
                                                      m_w.x, m_w.y, m_w.z, 0.0f,
                                                      0.0f, 0.0f, 0.0f, 1.0f));

    m_view_mat = look_mat * pos_mat;
}

void Camera::rotateV(float degrees)
{
    glm::vec3 u, v, w;
    u = m_u;
    v = m_v;
    w = m_w;
    m_u = glm::normalize(u*glm::cos(glm::radians(degrees)) - w*glm::sin(glm::radians(degrees)));
    m_v = glm::normalize(v);
    m_w = glm::normalize(u*glm::sin(glm::radians(degrees)) + w*glm::cos(glm::radians(degrees)));

    glm::mat4x4 pos_mat = glm::transpose(glm::mat4x4(1.0f, 0.0f, 0.0f, -m_eye.x,
                                                     0.0f, 1.0f, 0.0f, -m_eye.y,
                                                     0.0f, 0.0f, 1.0f, -m_eye.z,
                                                     0.0f, 0.0f, 0.0f, 1.0f));



    glm::mat4x4 look_mat = glm::transpose(glm::mat4x4(m_u.x, m_u.y, m_u.z, 0.0f,
                                                      m_v.x, m_v.y, m_v.z, 0.0f,
                                                      m_w.x, m_w.y, m_w.z, 0.0f,
                                                      0.0f, 0.0f, 0.0f, 1.0f));

    m_view_mat = look_mat * pos_mat;
}


void Camera::setClip(float nearPlane, float farPlane)
{

    m_near_plane = nearPlane;
    m_far_plane = farPlane;

    m_scale_mat = glm::transpose(glm::mat4x4(float(1.0/(m_far_plane*tan(m_height_angle/2.0)*m_aspect_ratio)), 0.0f, 0.0f, 0.0f,
                            0.0f, float(1.0/(m_far_plane*tan(m_height_angle/2.0))), 0.0f, 0.0f,
                            0.0f, 0.0f, float(1.0/m_far_plane), 0.0f,
                            0.0f, 0.0f, 0.0f, 1.0f));
    float c = -m_near_plane/m_far_plane;
    m_persp_mat = glm::transpose(glm::mat4x4(1.0f, 0.0f, 0.0f, 0.0f,
                       0.0f, 1.0f, 0.0f, 0.0f,
                       0.0f, 0.0f, -1.0/(1 + c), c/(1 + c),
                       0.0f, 0.0f, -1.0f, 0.0f));
    m_proj_mat = m_persp_mat * m_scale_mat;
}

bool Camera::CastRayAtObject(glm::vec3 *hit, glm::mat4x4 model) {
    glm::mat4x4 inv = glm::inverse(model);
    glm::vec4 model_dir = inv*m_look;
    glm::vec4 model_eye = inv*m_eye;

    float t = (0.5 - model_eye.y)/model_dir.y;

    *hit = glm::vec3(model_dir*t + model_eye);

    bool on_face = (hit->x >= -0.5 && hit->x <= 0.5 &&
                    hit->z >= -0.5 && hit->z <= 0.5);
    return (t >= 0 && on_face);
}


void Camera::MouseMoved(int dx, int dy) {
    // compute the new look vector based upon the old one
    float x_ratio = dx/float(m_px_w/2.0);      float y_ratio = dy/float(m_px_h/2.0);
    float real_width = m_near_plane*tan((m_height_angle*m_aspect_ratio)/2.0);  float real_height = m_near_plane*tan(m_height_angle/2.0);
    float new_width = real_width * x_ratio;    float new_height = real_height * y_ratio;
    float x_angle = atan(new_width/m_near_plane);    float y_angle = atan(new_height/m_near_plane);

    float factor = 2.0f;


    rotateV(glm::degrees(-x_angle * factor));
    rotateU(glm::degrees(-y_angle * factor));


}

void Camera::MouseScrolled(int units) {
    float factor = 1.0/1200.0f;
    //glm::mat4x4 transform = glm::translate(-factor * float(units) * look);
    translate(factor * float(units) * glm::vec4(m_w, 0.0f));

}
