#ifndef SETTINGS_H
#define SETTINGS_H

#include <iostream>

//Controls
#define CAMERA_SENSITIVITY (.2f)
#define CLICK_IMPULSE (glm::vec3(0,-0.05,0))
#define DEFAULT_TICK_LENGTH (1.f/60.f)
#define KEYPRESS_FOR_TICK (false)
#define SCROLL_TICK .0002f

//Camera Settings
#define ASPECT_RATIO 16.f/9.f
#define FAR_PLANE 3.f
#define HEIGHT_ANGLE 60.f
#define NEAR_PLANE 0.001f
//  different types of default camera placements
//  default is looking directly down upon the water
#define LOOK_SETTING_ACROSS false


// Cubemap Settings
#define LEFT_IMG "emerald/emerald_right.jpg"
#define RIGHT_IMG "emerald/emerald_left.jpg"
#define TOP_IMG "emerald/emerald_top.jpg"
#define BOTTOM_IMG "emerald/nopeeking.jpg"
#define FRONT_IMG "emerald/emerald_front.jpg"
#define BACK_IMG "emerald/emerald_back.jpg"


//Render Settings
#define POLYGON_MODE false
#define SPHERE_RESOLUTION (50)
#define USE_CUBE_MAP true
#define WATER_RESOLUTION (500)

//Lighting Settings
#define GLOBAL_AMBIENT_COEFFICIENT 0.2f
#define GLOBAL_AMBIENT_INTENSITY glm::vec3(0.25, 0.25, 0.25)
#define GLOBAL_DIFFUSE_COEFFICIENT 0.8f
#define SPHERE_AMBIENT_COLOR glm::vec3(1.0, 0.6, 0.6)
#define SPHERE_DIFFUSE_COLOR glm::vec3(1.0, 0.2, 0.2)
#define WATER_AMBIENT_COLOR glm::vec3(0.36882352941176473, 0.57470588235294116, 0.53725490196078434)
#define WATER_DIFFUSE_COLOR glm::vec3(0.36882352941176473, 0.57470588235294116, 0.53725490196078434)

//Water Surface - Interactions
#define MAX_BUOYANCY (100)
#define SUPPORT_COEFFICIENT 30.f
#define SUPPORTED_VELOCITY_COEFFICIENT .1f
#define SURFACE_IMPULSE_COEFFICIENT .15f
#define SURFACE_SIDEWAYS_COEFFICIENT 0.015f

//Water Surface - Settings
#define BOTTOM_EDGE_HEIGHT .002f
#define IMPULSE_CAP 1.5f
#define LEFT_EDGE_HEIGHT .0015f
#define MINIMUM_VELOCITY (-1.5f)
#define MAXIMUM_VELOCITY (1.5f)
#define RIGHT_EDGE_HEIGHT .001f
#define TOP_EDGE_HEIGHT .0005f
#define VELOCITY_AVERAGE_FACTOR (2.f)
#define VELOCITY_DAMPING_FACTOR (.993f)
#define WATER_PLANE_HEIGHT -.12f

//Object - Interactions
#define GRAVITY (glm::vec3(0,-.2,0))
#define SPHERE_BUOYANCY (70.f)
#define SPHERE_COR (.2)
#define MAX_BALLS 3

//Objects - Settings
#define SPHERE_MASS (100)

#define EQ(a,b) (((a)-(b))<.00001f)&&(((a)-(b))>-.00001f)


#endif // SETTINGS_H
