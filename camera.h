#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"
#include "settings.h"

class Camera
{
public:
    Camera(int px_w, int px_h);
    void MouseMoved(int dx, int dy);
    void MouseScrolled(int units);

    void ComputeView();
    bool CastRayAtObject(glm::vec3 *hit, glm::mat4x4 model);

    virtual void setAspectRatio(float aspectRatio);

        virtual float getAspectRatio();
        virtual glm::vec4 getEye();
        // Returns the projection matrix given the current camera settings.
        virtual glm::mat4x4 getProjectionMatrix() const;

        // Returns the view matrix given the current camera settings.
        virtual glm::mat4x4 getViewMatrix() const;

        // Returns the matrix that scales down the perspective view volume into the canonical
        // perspective view volume, given the current camera settings.
        virtual glm::mat4x4 getScaleMatrix() const;

        // Returns the matrix the unhinges the perspective view volume, given the current camera
        // settings.
        virtual glm::mat4x4 getPerspectiveMatrix() const;

        // Returns the current position of the camera.
        glm::vec4 getPosition() const;

        // Returns the current 'look' vector for this camera.
        glm::vec4 getLook() const;

        // Returns the current 'up' vector for this camera (the 'V' vector).
        glm::vec4 getUp() const;

        // Returns the currently set aspect ratio.
        float getAspectRatio() const;

        // Returns the currently set height angle.
        float getHeightAngle() const;

        // Move this camera to a new eye position, and orient the camera's axes given look and up
        // vectors.
        void orientLook(const glm::vec4 &eye, const glm::vec4 &look, const glm::vec4 &up);

        // Sets the height angle of this camera.
        void setHeightAngle(float h);

        // Translates the camera along a given vector.
        void translate(const glm::vec4 &v);

        // Rotates the camera about the U axis by a specified number of degrees.
        void rotateU(float degrees);

        // Rotates the camera about the V axis by a specified number of degrees.
        void rotateV(float degrees);

        // Rotates the camera about the W axis by a specified number of degrees.
        void rotateW(float degrees);

        // Sets the near and far clip planes for this camera.
        void setClip(float nearPlane, float farPlane);



    int m_px_w, m_px_h;

      float heightAngle = 60.0f, aspectRatio = 1.0f, nearPlane = 1.0f, farPlane = 30.0f;
      glm::mat4x4 projectionMatrix, viewMatrix,
                  scaleMatrix, perspectiveMatrix;
      glm::vec4 look, up, eye;
      glm::vec3 u, v, w;
      void calculateView();
      void calculateScale();

};


#endif // CAMERA_H
