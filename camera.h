#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"

class Camera
{
public:
    Camera(int px_w, int px_h);

    glm::mat4x4 P();
    glm::mat4x4 V();

    void MouseMoved(int dx, int dy);
    void MouseScrolled(int units);

    bool CastRayAtObject(glm::vec3 *hit, glm::mat4x4 model);

private:

    glm::mat4x4 m_p, m_v;

    glm::vec3 m_eye, m_target, m_up;
    float m_fov;
    float m_aspect;
    float m_near;
    float m_far;

    int m_px_w, m_px_h;

};

#endif // CAMERA_H
