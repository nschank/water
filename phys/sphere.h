#ifndef SPHERE_H
#define SPHERE_H

#include "entity.h"

#define SPHERE_MASS (10)
#define SPHERE_COR (.5)

class Sphere : public Entity
{
friend class WaterSurface;

public:
	Sphere(glm::vec3 worldLocation, float radius);
	virtual ~Sphere();

	void collideWith(Entity *other);

	void applyTranslationAt(glm::vec3 translation, glm::vec3 location);
	void applyImpulseAt(glm::vec3 impulse, glm::vec3 location);
	void applyForceAt(glm::vec3 force, glm::vec3 location);
	void tick(float secondsSinceLastTick);

protected:
	void collideWithSphere(Sphere *other);
	void collideWithSurface(WaterSurface *other);

	glm::vec3 m_center;
	glm::vec3 m_velocity;
	float m_radius;
	float m_mass;
	float m_cor;

	glm::vec3 impulsesThisTick;
	glm::vec3 forcesThisTick;
};

#endif // SPHERE_H
