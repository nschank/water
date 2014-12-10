#ifndef ENTITY_H
#define ENTITY_H

#include "glm.hpp"

class SphereEntity;
class WaterSurface;

class Entity
{
friend class SphereEntity;
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
	virtual void collideWithSphere(SphereEntity *other) = 0;
	virtual void collideWithSurface(WaterSurface *other) = 0;
};

#endif // ENTITY_H
