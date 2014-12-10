#include "sphereentity.h"
#include "watersurface.h"
#include <stdio.h>
#include <iostream>

#define FPS (60)
#define MAX_MASS (100)

SphereEntity::SphereEntity(glm::vec3 worldLocation, float radius) :
	m_center(worldLocation), m_radius(radius), m_mass(SphereEntity_MASS), m_cor(SphereEntity_COR)
{
	this->updateMatrices();
}


SphereEntity::~SphereEntity()
{

}

void SphereEntity::collideWith(Entity *other)
{
	return other->collideWithSphere(this);
}

void SphereEntity::collideWithSphere(SphereEntity *other)
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

	applyTranslationAt((m_radius+other->m_radius - distance)*meToOtherUnitMTV* -myMoveWeight, m_center);
	other->applyTranslationAt((m_radius+other->m_radius - distance)*meToOtherUnitMTV*otherMoveWeight, other->m_center);
}

void SphereEntity::collideWithSurface(WaterSurface *surface)
{
	if(m_center.y - m_radius > surface->getMaxHeight()) //ball is too high up
		return;

	//the number of points necessary to hold up this ball precisely
	const float buoyancy = (.5f*m_mass/MAX_MASS + .5f) * M_PI * m_radius * m_radius / surface->getXResolution() / surface->getYResolution();

	const float max_x = glm::min(.5f,m_radius + m_center.x+surface->getXResolution());
	const float max_z = glm::min(.5f,m_radius + m_center.z+surface->getYResolution());

	int supportingPoints = 0;
	glm::vec3 supportingVector;

	//x2+y2+z2=r2
	for(float x = glm::max(-.5f,m_center.x - m_radius); x <= max_x; x += surface->getXResolution())
	{
		const float x_dist = (x - m_center.x)*(x - m_center.x);
		for(float z = glm::max(-.5f,m_center.z - m_radius); z <= max_z; z += surface->getYResolution())
		{
			float y = surface->heightAt(x,z);
			float y_dist = (y - m_center.y)*(y - m_center.y);
			float z_dist = (z - m_center.z)*(z - m_center.z);

			float distance = glm::sqrt(x_dist + y_dist + z_dist);
			if(distance > m_radius)
				continue;

			float ball_y = glm::sqrt(m_radius*m_radius - x_dist - y_dist) + m_center.y;

			supportingPoints++;
			supportingVector += glm::vec3(x,ball_y,z) - m_center;
			surface->applyImpulseAt(glm::vec3(0,ball_y-y+.001,0), glm::vec3(x,0,z));
		}
	}

	if(supportingPoints > 0)
	{
		supportingVector /= supportingVector.y;

		this->applyTranslationAt(glm::vec3(0,.001,0), m_center);
		this->applyImpulseAt(-m_velocity.y * m_mass * float(supportingPoints)/buoyancy * supportingVector, m_center);
	}
}

void SphereEntity::applyTranslationAt(glm::vec3 translation, glm::vec3 location)
{
	m_center += translation;
}

void SphereEntity::applyImpulseAt(glm::vec3 impulse, glm::vec3 location)
{
	impulsesThisTick += impulse;
}

void SphereEntity::applyForceAt(glm::vec3 force, glm::vec3 location)
{
	forcesThisTick += force;
}

void SphereEntity::tick(float secondsSinceLastTick)
{
	if(m_mass > 0)
		m_velocity += (impulsesThisTick + (forcesThisTick * secondsSinceLastTick))/m_mass;
	impulsesThisTick = glm::vec3();
	forcesThisTick = glm::vec3();

	m_center += (m_velocity * secondsSinceLastTick);
	updateMatrices();
}

glm::mat4 SphereEntity::modelMatrix()
{
	return m_modelMatrix;
}

glm::mat3 SphereEntity::normalMatrix()
{
	return m_normalMatrix;
}

void SphereEntity::updateMatrices()
{
	m_modelMatrix = glm::translate(m_center)*glm::scale(glm::vec3(m_radius*2));
	m_normalMatrix = glm::mat3x3(glm::transpose(glm::inverse(m_modelMatrix)));
}
