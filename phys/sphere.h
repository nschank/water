#ifndef SPHERE_H
#define SPHERE_H

#include "entity.h"

#define SPHERE_MASS (10)

class Sphere : public Entity
{
friend class WaterSurface;

public:
	Sphere(glm::vec3 worldLocation, float radius);
	virtual ~Sphere();

	Collision *collisionWith(Entity *other);

	void applyTranslationAt(glm::vec3 translation, glm::vec3 location);
	void applyImpulseAt(glm::vec3 impulse, glm::vec3 location);
	void applyForceAt(glm::vec3 force, glm::vec3 location);
	void tick(float secondsSinceLastTick);

protected:
	Collision *collisionWithSphere(Sphere *other);
	Collision *collisionWithSurface(WaterSurface *other);

	glm::vec3 m_center;
	glm::vec3 m_velocity;
	float m_radius;
	float m_mass;

	glm::vec3 impulsesThisTick;
	glm::vec3 forcesThisTick;
};

#endif // SPHERE_H
