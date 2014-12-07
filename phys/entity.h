#ifndef ENTITY_H
#define ENTITY_H

#include "collision.h"

class Sphere;

class Entity
{
friend class Sphere;

public:
	Entity();
	virtual ~Entity();

	virtual Collision *collisionWith(Entity *other) = 0;

protected:
	virtual Collision *collisionWithSphere(Sphere *other) = 0;
};

#endif // ENTITY_H
