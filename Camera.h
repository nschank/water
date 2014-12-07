#ifndef CAMERA_H
#define CAMERA_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

/**
 * @class Camera
 *
 * An abstract class representing a virtual camera with an optional mouse interaction.
 */
class Camera
{
public:
    // Called when the window size changes
    virtual void setAspectRatio(float) = 0;

    // C++ Note: The following are pure virtual functions - they must be
    // overridden in subclasses.

    // Return the projection matrix for the current camera settings.
    virtual glm::mat4x4 getProjectionMatrix() const = 0;

    // Return the view matrix for the current camera settings.
    virtual glm::mat4x4 getViewMatrix() const = 0;
    
    virtual glm::mat4x4 getM2() const = 0;

    virtual glm::vec4 getEye() const = 0;

    //
    // The following functions are called for mouse events on the 3D canvas tab.
    //
    // C++ Note: Why are the following methods not pure virtual? If they are not overridden in
    // subclasses, then the empty implementation here ({}) will take over.
    //

    // Called when the mouse button is pressed down.
    virtual void mouseDown(int x, int y)    {}

    // Called when the mouse button is dragged.
    virtual void mouseDragged(int x, int y) {}

    // Called when the mouse button is released.
    virtual void mouseUp(int x, int y)      {}

    // Called when the scroll wheel changes position.
    virtual void mouseScrolled(int delta)   {}
};

#endif // CAMERA_H
