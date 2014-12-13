#ifndef SETTINGS_H
#define SETTINGS_H

#include <iostream>

//Controls
#define CLICK_IMPULSE (glm::vec3(0,-0.05,0))
#define DEFAULT_TICK_LENGTH (1.f/60.f)
#define KEYPRESS_FOR_TICK (false)
#define SCROLL_TICK .0005f

//Camera Settings
#define FAR_PLANE 3.f
#define HEIGHT_ANGLE 60.f
#define NEAR_PLANE 0.001f
//  different types of default camera placements
//  default is looking directly down upon the water
#define LOOK_SETTING_ACROSS true


// Cubemap Settings
/*#define LEFT_IMG "lagoon_dn.tga"
#define RIGHT_IMG "lagoon_rt.tga"
#define TOP_IMG "lagoon_dn.tga"
#define BOTTOM_IMG "lagoon_dn.tga"
#define FRONT_IMG "lagoon_ft.tga"
#define BACK_IMG "lagoon_bk.tga"*/

/*#define LEFT_IMG "sor_lake1/lake1_lf.JPG"
#define RIGHT_IMG "sor_lake1/lake1_rt.JPG"
#define TOP_IMG "sor_lake1/lake1_up.JPG"
#define BOTTOM_IMG "sor_lake1/lake1_dn.JPG"
#define FRONT_IMG "sor_lake1/lake1_ft.JPG"
#define BACK_IMG "sor_lake1/lake1_bk.JPG"*/

#define LEFT_IMG "emerald/emerald_right.jpg"
#define RIGHT_IMG "emerald/emerald_left.jpg"
#define TOP_IMG "emerald/emerald_top.jpg"
#define BOTTOM_IMG "sor_lake1/lake1_dn.JPG"
#define FRONT_IMG "emerald/emerald_front.jpg"
#define BACK_IMG "emerald/emerald_back.jpg"


//Render Settings
#define POLYGON_MODE false
#define SPHERE_RESOLUTION (30)
#define USE_CUBE_MAP true
#define WATER_RESOLUTION (300)

//Lighting Settings
#define GLOBAL_AMBIENT_COEFFICIENT 0.2f
#define GLOBAL_AMBIENT_INTENSITY glm::vec3(0.25, 0.25, 0.25)
#define GLOBAL_DIFFUSE_COEFFICIENT 0.8f
#define SPHERE_AMBIENT_COLOR glm::vec3(1.0, 0.6, 0.6)
#define SPHERE_DIFFUSE_COLOR glm::vec3(1.0, 0.2, 0.2)
#define WATER_AMBIENT_COLOR glm::vec3(0.0, 0.8, 1.0)
#define WATER_DIFFUSE_COLOR glm::vec3(0.0, 0.8, 1.0)

//Water Surface - Interactions
#define MAX_BUOYANCY (100)
#define SUPPORT_COEFFICIENT 20.f
#define SUPPORTED_VELOCITY_COEFFICIENT .075f
#define SURFACE_IMPULSE_COEFFICIENT .15f
#define SURFACE_SIDEWAYS_COEFFICIENT 0.03f

//Water Surface - Settings
#define MINIMUM_VELOCITY (-1.5f)
#define MAXIMUM_VELOCITY (1.5f)
#define VELOCITY_AVERAGE_FACTOR (2.0f)
#define VELOCITY_DAMPING_FACTOR (.9995f)

//Object - Interactions
#define GRAVITY (glm::vec3(0,-.098,0))
#define SPHERE_BUOYANCY (70.f)
#define SPHERE_COR (.2)

//Objects - Settings
#define SPHERE_MASS (100)

#define EQ(a,b) (((a)-(b))<.00001f)&&(((a)-(b))>-.00001f)


#endif // SETTINGS_H