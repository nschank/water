#ifndef SphereEntity_H
#define SphereEntity_H

#include "entity.h"

#define SphereEntity_MASS (10)
#define SphereEntity_COR (.5)

class SphereEntity : public Entity
{
friend class WaterSurface;

public:
	SphereEntity(glm::vec3 worldLocation, float radius);
	virtual ~SphereEntity();

	void collideWith(Entity *other);

	void applyTranslationAt(glm::vec3 translation, glm::vec3 location);
	void applyImpulseAt(glm::vec3 impulse, glm::vec3 location);
	void applyForceAt(glm::vec3 force, glm::vec3 location);
	void tick(float secondsSinceLastTick);

protected:
	void collideWithSphere(SphereEntity *other);
	void collideWithSurface(WaterSurface *other);

	glm::vec3 m_center;
	glm::vec3 m_velocity;
	float m_radius;
	float m_mass;
	float m_cor;

	glm::vec3 impulsesThisTick;
	glm::vec3 forcesThisTick;
};

#endif // SphereEntity_H
