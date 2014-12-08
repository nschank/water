#ifndef ENTITY_H
#define ENTITY_H

#include "collision.h"

class Sphere;
class WaterSurface;

class Entity
{
friend class Sphere;
friend class WaterSurface;

public:
	Entity();
	virtual ~Entity();

	virtual Collision *collisionWith(Entity *other) = 0;

	virtual void applyTranslationAt(glm::vec3 translation, glm::vec3 location) = 0;
	virtual void applyImpulseAt(glm::vec3 impulse, glm::vec3 location) = 0;
	virtual void applyForceAt(glm::vec3 force, glm::vec3 location) = 0;

	virtual void tick(float secondsSinceLastTick) = 0;

protected:
	virtual Collision *collisionWithSphere(Sphere *other) = 0;
	virtual Collision *collisionWithSurface(WaterSurface *other) = 0;
};

#endif // ENTITY_H
