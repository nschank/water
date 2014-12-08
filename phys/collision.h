#ifndef COLLISION_H
#define COLLISION_H

#include "glm.hpp"

class Entity;

struct Collision
{
	glm::vec3 point;
	glm::vec3 mtv;

	Entity *of;
	Entity *with;

	void react()
	{
		//Apply location change

		//Apply impulse
	}
};

#endif // COLLISION_H
