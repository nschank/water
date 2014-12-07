/**
 * @file   OrbitingCamera.cpp
 *
 * (See the header file.) You don't need to be poking around in this file unless you're interested
 * in how an orbiting camera works.
 *
 * The way we have implemented this class is NOT how you should be implementing your Camtrans. This
 * camera is a DIFFERENT TYPE of camera which we're providing so you can easily view your Shapes
 * and to make sure your scene graph is working if your camera isn't.
 *
 * In the Camtrans lab, you'll be implementing your own perspective camera from scratch! This one
 * uses OpenGL.
 */

#include "OrbitingCamera.h"

#include <float.h>
#include <math.h>

#define CAMERA_NEAR 0.01f
#define CAMERA_FAR 100.0f

#define CAMERA_FOV 55.0f

#define CAMERA_POS_X 1.0f
#define CAMERA_POS_Y 1.0f
#define CAMERA_POS_Z 1.0f


OrbitingCamera::OrbitingCamera()
{
    m_aspectRatio = 1;
    m_angleX = m_angleY = 0;
    m_zoomZ = -5;
}

void OrbitingCamera::setAspectRatio(float aspectRatio)
{
    m_aspectRatio = aspectRatio;

    updateProjectionMatrix();
}

glm::mat4x4 OrbitingCamera::getProjectionMatrix() const
{
    return m_projectionMatrix;
}

glm::mat4x4 OrbitingCamera::getM2() const {
    
    float x = CAMERA_FAR * tan(CAMERA_FOV/4.0);
    glm::mat4x4 scale_mat = glm::transpose(glm::mat4x4(
        float(1.0/(x*m_aspectRatio)), 0.0f, 0.0f, 0.0f,
        0.0f, float(1.0/(x)), 0.0f, 0.0f,
        0.0f, 0.0f, float(1.0/CAMERA_FAR), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f));
    return scale_mat;
}

glm::mat4x4 OrbitingCamera::getViewMatrix() const
{
    return m_viewMatrix;
}

glm::vec4 OrbitingCamera::getEye() const {
    return glm::vec4(CAMERA_POS_X, CAMERA_POS_Y, CAMERA_POS_Z, 0.0f);
}

void OrbitingCamera::mouseDown(int x, int y)
{
    m_oldX = x;
    m_oldY = y;
}

void OrbitingCamera::mouseDragged(int x, int y)
{
    m_angleY += x - m_oldX;
    m_angleX += y - m_oldY;
    m_oldX = x;
    m_oldY = y;
    if (m_angleX < -90) m_angleX = -90;
    if (m_angleX > 90) m_angleX = 90;

    updateViewMatrix();
}

void OrbitingCamera::mouseScrolled(int delta)
{
    // Use an exponential factor so the zoom increments are small when we are
    // close to the object and large when we are far away from the object
    m_zoomZ *= powf(0.999f, delta);

    updateViewMatrix();
}

void OrbitingCamera::updateMatrices()
{
    updateProjectionMatrix();
    updateViewMatrix();
}

void OrbitingCamera::updateProjectionMatrix()
{
    // Make sure glm gets a far value that is greater than the near value.
    // Thanks Windows for making far a keyword!
    float farPlane = std::max(CAMERA_FAR, CAMERA_NEAR + 100.f * FLT_EPSILON);

    m_projectionMatrix = glm::perspective(
            glm::radians(CAMERA_FOV), m_aspectRatio, CAMERA_NEAR, farPlane);
}

void OrbitingCamera::updateViewMatrix()
{
    m_viewMatrix =
            glm::translate(glm::vec3(0.f, 0.f, m_zoomZ)) *
            glm::rotate(glm::radians(m_angleY), glm::vec3(0.f, 1.f, 0.f)) *
            glm::rotate(glm::radians(m_angleX), glm::vec3(1.f, 0.f, 0.f));
}
