#include "sphereentity.h"
#include "watersurface.h"
#include <stdio.h>
#include <iostream>

SphereEntity::SphereEntity(glm::vec3 worldLocation, float radius) :
	m_center(worldLocation), m_radius(radius), m_mass(SPHERE_MASS), m_cor(SPHERE_COR), m_buoyancy(SPHERE_BUOYANCY)
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
	if(m_center.y - m_radius > MAXIMUM_VELOCITY) //ball is too high up
		return;

	//the number of points necessary to hold up this ball precisely
	const float maximumArea = M_PI * m_radius * m_radius;
	const float requiredArea = (m_buoyancy/MAX_BUOYANCY) * maximumArea;

	glm::vec2 upperLeft = surface->closestDiscretePoint(m_center - glm::vec3(m_radius+surface->getResolution()));
	glm::vec2 lowerRight = surface->closestDiscretePoint(m_center + glm::vec3(m_radius+surface->getResolution()));

	int supportingPoints = 0;
	float diff = 0;
	float allPoints = (lowerRight.y - upperLeft.y + 1)*(lowerRight.x - upperLeft.x + 1);
	glm::vec3 supportingVector;

	//x2+y2+z2=r2
	for(int x_i = (int)upperLeft.x; x_i <= (int)lowerRight.x; x_i++)
	{
		const float x = -.5f + x_i*surface->getResolution();
		const float x_dist = (x - m_center.x)*(x - m_center.x);

		for(int z_i = (int)upperLeft.y; z_i <= (int)lowerRight.y; z_i++)
		{
			float z = -.5f + z_i*surface->getResolution();
			float y = surface->heightAt(glm::vec2(x_i,z_i));
			float y_dist = m_center.y > y ? (y - m_center.y)*(y - m_center.y) : 0;
			float z_dist = (z - m_center.z)*(z - m_center.z);

			if(glm::sqrt(x_dist + y_dist + z_dist) > m_radius)
				continue; // ball is not touching or below water here

			float ball_y = -glm::sqrt(m_radius*m_radius - x_dist - z_dist) + m_center.y;

			supportingPoints++;
			supportingVector += glm::vec3(x,ball_y,z) - m_center;
			diff += y-ball_y;

			float impulseStrength = ball_y-y;
			//surface->applyImpulseAt(glm::vec3(0,impulseStrength,0), glm::vec3(x_i,0,z_i));

			surface->setPoint(glm::vec2(x_i,z_i), ball_y);
		}
	}

	if(supportingPoints > 0)
	{
		supportingVector /= supportingVector.y;

		diff = 0;

		supportingVector.x = 0;
		supportingVector.z = 0;

		float velocityCoefficient;
		if(m_velocity.y > 0) velocityCoefficient = -m_velocity.y/2 + diff;
		else velocityCoefficient = -m_velocity.y + diff;

		float support = float(supportingPoints)/allPoints*M_PI/4;

		//this->applyTranslationAt(glm::vec3(0,.001,0), m_center);
		this->applyImpulseAt(velocityCoefficient * support/requiredArea * supportingVector, m_center);
		if(m_center.y < 0)
			this->applyForceAt(BUOYANCY_COEFFICIENT * glm::vec3(0,-m_center.y,0), m_center);
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
