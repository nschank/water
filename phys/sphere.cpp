#include "sphere.h"

Sphere::Sphere(glm::vec3 worldLocation, float radius) :
	m_center(worldLocation), m_radius(radius), m_mass(SPHERE_MASS)
{
}


Sphere::~Sphere()
{

}

Collision *Sphere::collisionWith(Entity *other)
{
	return other->collisionWithSphere(this);
}

Collision *Sphere::collisionWithSphere(Sphere *other)
{
	float distance = glm::distance(m_center, other->m_center);

	//No collision
	if(distance > m_radius + other->m_radius)
		return NULL;

	return NULL;
}

Collision *Sphere::collisionWithSurface(WaterSurface *other)
{
	return NULL;
}

void Sphere::applyTranslationAt(glm::vec3 translation, glm::vec3 location)
{
	m_center += translation;
}

void Sphere::applyImpulseAt(glm::vec3 impulse, glm::vec3 location)
{
	impulsesThisTick += impulse;
}

void Sphere::applyForceAt(glm::vec3 force, glm::vec3 location)
{
	forcesThisTick += force;
}

void Sphere::tick(float secondsSinceLastTick)
{
	m_velocity += (impulsesThisTick + (forcesThisTick * secondsSinceLastTick))/m_mass;
	impulsesThisTick = glm::vec3();
	forcesThisTick = glm::vec3();

	m_center += (m_velocity * secondsSinceLastTick);
}
