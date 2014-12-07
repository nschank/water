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
};

#endif // COLLISION_H
