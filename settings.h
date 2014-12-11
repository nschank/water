#ifndef SETTINGS_H
#define SETTINGS_H

#include <iostream>

//Controls
#define CLICK_IMPULSE (glm::vec3(0,-.5,0))
#define DEFAULT_TICK_LENGTH (1.f/60.f)
#define KEYPRESS_FOR_TICK false

//Render Settings
#define POLYGON_MODE true
#define USE_CUBE_MAP true

//Lighting Settings
#define GLOBAL_AMBIENT_COEFFICIENT 0.2f
#define GLOBAL_AMBIENT_INTENSITY glm::vec3(0.25, 0.25, 0.25)
#define GLOBAL_DIFFUSE_COEFFICIENT 0.8f
#define SPHERE_AMBIENT_COLOR glm::vec3(1.0, 0.6, 0.6)
#define SPHERE_DIFFUSE_COLOR glm::vec3(1.0, 0.2, 0.2)
#define WATER_AMBIENT_COLOR glm::vec3(0.0, 0.8, 1.0)
#define WATER_DIFFUSE_COLOR glm::vec3(0.0, 0.6, 1.0)

//Water Surface - Interactions


//Water Surface - Settings
#define MINIMUM_VELOCITY (-1.5f)
#define MAXIMUM_VELOCITY (1.5f)
#define VELOCITY_AVERAGE_FACTOR (2.0f)
#define VELOCITY_DAMPING_FACTOR (.95f)
#define WATER_RESOLUTION (50)

//Object - Interactions
#define GRAVITY (glm::vec3(0,-9.8,0))

//Objects - Settings
#define SPHERE_RESOLUTION (30)

#endif // SETTINGS_H
