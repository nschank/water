#include "sphere.h"
#include "watersurface.h"

Sphere::Sphere(glm::vec3 worldLocation, float radius) :
	m_center(worldLocation), m_radius(radius), m_mass(SPHERE_MASS), m_cor(SPHERE_COR)
{
}


Sphere::~Sphere()
{

}

void Sphere::collideWith(Entity *other)
{
	return other->collideWithSphere(this);
}

void Sphere::collideWithSphere(Sphere *other)
{
	float distance = glm::distance(m_center, other->m_center);

	//No collision
	if(distance > m_radius + other->m_radius)
		return;

	glm::vec3 meToOther = other->m_center - m_center;

	glm::vec3 meToOtherUnitMTV = meToOther/distance;

	glm::vec3 myProjVelocity = m_center + (meToOtherUnitMTV * glm::dot(meToOtherUnitMTV, m_velocity-m_center));
	glm::vec3 otherProjVelocity = m_center + (meToOtherUnitMTV * glm::dot(meToOtherUnitMTV, other->m_velocity-m_center));

	glm::vec3 unweightedVelocityChange = otherProjVelocity - myProjVelocity;

	float weight = (m_mass * other->m_mass * (1+m_cor*other->m_cor)) / (m_mass + other->m_mass);

	glm::vec3 myImpulse = unweightedVelocityChange * weight;

	this->applyImpulseAt(myImpulse, m_center);
	other->applyImpulseAt(-1.f*myImpulse, other->m_center);

	float myMoveWeight = other->m_mass/(m_mass + other->m_mass);
	float otherMoveWeight = 1 - myMoveWeight;

	applyTranslationAt(distance*meToOtherUnitMTV* -myMoveWeight, m_center);
	other->applyTranslationAt(distance*meToOtherUnitMTV*otherMoveWeight, other->m_center);
}

void Sphere::collideWithSurface(WaterSurface *surface)
{
	/*if(m_center.z - m_radius > surface->getMaxHeight()) //ball is too high up
		return;

	/*float collisionWeight = 0;
	glm::vec3 impulsePosition;

	const float max_x = surface->getXResolution()*m_radius + m_center.x;
	const float max_y = surface->getYResolution()*m_radius + m_center.y;
	const float radius2 = m_radius*m_radius;

	for(float x = m_center.x - surface->getXResolution()*m_radius; x < max_x; x += surface->getXResolution())
	{
		const float x_dist = (x - m_center.x)*(x - m_center.x);
		for(float y = m_center.y - surface->getYResolution()*m_radius; y < max_y; y += surface->getYResolution())
		{
			float z = surface->heightAt(x,y);
			float yz_dist = (y - m_center.y)*(y - m_center.y) + (z - m_center.z)*(z - m_center.z);

			if(x_dist + yz_dist > radius2)


		}
	}*/

	if(m_center.y > m_radius) return;

	applyImpulseAt(glm::vec3(0,-m_velocity.y,0),m_center);
	m_center.y = m_radius;
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
