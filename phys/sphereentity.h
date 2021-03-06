#ifndef SphereEntity_H
#define SphereEntity_H

#include "entity.h"

class SphereEntity : public Entity
{
friend class WaterSurface;
friend class View;

public:
	SphereEntity(glm::vec3 worldLocation, float radius);
	virtual ~SphereEntity();

	void collideWith(Entity *other);

	void applyTranslationAt(glm::vec3 translation, glm::vec3 location);
	void applyImpulseAt(glm::vec3 impulse, glm::vec3 location);
	void applyForceAt(glm::vec3 force, glm::vec3 location);
	void tick(float secondsSinceLastTick);

	glm::mat3 normalMatrix();
	glm::mat4 modelMatrix();

	glm::vec3 m_velocity;

protected:
	void collideWithSphere(SphereEntity *other);
	void collideWithSurface(WaterSurface *other);
	void updateMatrices();

	glm::vec3 m_center;

	float m_radius;
	float m_mass;
	float m_cor;
	float m_buoyancy;

	glm::vec3 impulsesThisTick;
	glm::vec3 forcesThisTick;

	glm::mat3 m_normalMatrix;
	glm::mat4 m_modelMatrix;
};

#endif // SphereEntity_H
