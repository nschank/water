#ifndef ENTITY_H
#define ENTITY_H

#include "glm.hpp"

class Sphere;
class WaterSurface;

class Entity
{
friend class Sphere;
friend class WaterSurface;

public:
	Entity();
	virtual ~Entity();

	virtual void collideWith(Entity *other) = 0;

	virtual void applyTranslationAt(glm::vec3 translation, glm::vec3 location) = 0;
	virtual void applyImpulseAt(glm::vec3 impulse, glm::vec3 location) = 0;
	virtual void applyForceAt(glm::vec3 force, glm::vec3 location) = 0;

	virtual void tick(float secondsSinceLastTick) = 0;

protected:
	virtual void collideWithSphere(Sphere *other) = 0;
	virtual void collideWithSurface(WaterSurface *other) = 0;
};

#endif // ENTITY_H
