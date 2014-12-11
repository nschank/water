#ifndef SETTINGS_H
#define SETTINGS_H

#include <iostream>

//Controls
#define CLICK_IMPULSE (glm::vec3(0,-.2,0))
#define DEFAULT_TICK_LENGTH (1.f/60.f)
#define KEYPRESS_FOR_TICK (false)

//Camera Settings
#define FAR_PLANE 3.f
#define HEIGHT_ANGLE 60.f
#define NEAR_PLANE 0.001f
//  different types of default camera placements
//  default is looking directly down upon the water
#define LOOK_SETTING_ACROSS true
#define QTCREATOR_PLEASE_COMPILE_YOU_ASSHOLE .0005f

//Render Settings
#define POLYGON_MODE true
#define SPHERE_RESOLUTION (30)
#define USE_CUBE_MAP true
#define WATER_RESOLUTION (75)

//Lighting Settings
#define GLOBAL_AMBIENT_COEFFICIENT 0.2f
#define GLOBAL_AMBIENT_INTENSITY glm::vec3(0.25, 0.25, 0.25)
#define GLOBAL_DIFFUSE_COEFFICIENT 0.8f
#define SPHERE_AMBIENT_COLOR glm::vec3(1.0, 0.6, 0.6)
#define SPHERE_DIFFUSE_COLOR glm::vec3(1.0, 0.2, 0.2)
#define WATER_AMBIENT_COLOR glm::vec3(0.0, 0.8, 1.0)
#define WATER_DIFFUSE_COLOR glm::vec3(0.0, 0.6, 1.0)

//Water Surface - Interactions
#define BUOYANCY_COEFFICIENT (5.f)
#define MAX_BUOYANCY (100)

//Water Surface - Settings
#define MINIMUM_VELOCITY (-1.5f)
#define MAXIMUM_VELOCITY (1.5f)
#define VELOCITY_AVERAGE_FACTOR (3.0f)
#define VELOCITY_DAMPING_FACTOR (.9f)

//Object - Interactions
#define GRAVITY (glm::vec3(0,-9.8,0))
#define SPHERE_BUOYANCY (10.f)
#define SPHERE_COR (.2)

//Objects - Settings
#define SPHERE_MASS (90)


#endif // SETTINGS_H
