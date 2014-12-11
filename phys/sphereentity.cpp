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
	if(m_center.y - m_radius > 1.5) //ball is too high up
		return;

	glm::vec2 upperLeft = surface->closestDiscretePoint(m_center - glm::vec3(m_radius+surface->getResolution()));
	glm::vec2 lowerRight = surface->closestDiscretePoint(m_center + glm::vec3(m_radius+surface->getResolution()));

	int supportingPoints = 0;
	int possiblePoints = 0;
	glm::vec3 supportingVector;

	//x2+y2+z2=r2
	for(int x_i = (int)upperLeft.x; x_i <= (int)lowerRight.x; x_i++)
	{
		const float x = -.5f + x_i*surface->getResolution();
		const float x_dist = (x - m_center.x)*(x - m_center.x);

		for(int z_i = (int)upperLeft.y; z_i <= (int)lowerRight.y; z_i++)
		{
			float z = -.5f + z_i*surface->getResolution();
			float z_dist = (z - m_center.z)*(z - m_center.z);

			if(glm::sqrt(x_dist+z_dist) > m_radius)
				continue; //inside square, not circle here

			float y = surface->heightAt(glm::vec2(x_i,z_i));
			float y_dist = m_center.y >= y ? (y - m_center.y)*(y - m_center.y) : 0;
			possiblePoints++;

			if(glm::sqrt(x_dist + y_dist + z_dist) > m_radius)
				continue; // ball is not touching or above water here

			float ball_y = -glm::sqrt(m_radius*m_radius - x_dist - z_dist) + m_center.y;
			if(glm::isnan(ball_y)) ball_y = m_center.y;

			supportingPoints++;
			supportingVector += 1.f/(.01f+glm::abs(y))*(m_center - glm::vec3(x,y,z));

			surface->setPoint(glm::vec2(x_i,z_i), ball_y);
		}
	}

	if(supportingPoints > 0)
	{
		supportingVector /= supportingVector.y;

		glm::vec2 closestImpulseLocation = surface->closestDiscretePoint(m_center - supportingVector*m_radius);

		const float maximumArea = M_PI * m_radius * m_radius;
		const float requiredArea =  maximumArea * (1-m_buoyancy/MAX_BUOYANCY);

		const float proportionOfCircleSampled = float(supportingPoints)/float(possiblePoints);
		const float areaOfCircleSampled = M_PI * (m_radius+surface->getResolution()) * (m_radius+surface->getResolution());
		const float areaUsed = proportionOfCircleSampled*areaOfCircleSampled;

		const float support = 2.f /
				(1.f + glm::exp((requiredArea-areaUsed)/requiredArea*SUPPORT_COEFFICIENT));

		if(areaUsed >= requiredArea)
		{
			//We should support the velocity as much as we are allowed.
			applyImpulseAt(-SUPPORTED_VELOCITY_COEFFICIENT * m_velocity.y * m_mass * supportingVector, m_center);
			//We should experience an upwards force relative to how much area is used
			applyForceAt(-GRAVITY * m_mass * support, m_center);
		}
		else
		{
			applyImpulseAt(-SUPPORTED_VELOCITY_COEFFICIENT * m_velocity.y * m_mass * support * supportingVector, m_center);
			applyForceAt(-GRAVITY * m_mass * support * SUPPORTED_VELOCITY_COEFFICIENT, m_center);
		}

		//we apply an impulse to the water
		surface->applyImpulseAt(glm::vec3(0,-m_velocity.y*SURFACE_IMPULSE_COEFFICIENT,0),
								glm::vec3(closestImpulseLocation.x, 0, closestImpulseLocation.y));
		surface->applyImpulseAt(glm::vec3(0,-m_velocity.y/4.f*SURFACE_IMPULSE_COEFFICIENT,0),
								glm::vec3(closestImpulseLocation.x+1, 0, closestImpulseLocation.y+1));
		surface->applyImpulseAt(glm::vec3(0,-m_velocity.y/4.f*SURFACE_IMPULSE_COEFFICIENT,0),
								glm::vec3(closestImpulseLocation.x+1, 0, closestImpulseLocation.y-1));
		surface->applyImpulseAt(glm::vec3(0,-m_velocity.y/4.f*SURFACE_IMPULSE_COEFFICIENT,0),
								glm::vec3(closestImpulseLocation.x-1, 0, closestImpulseLocation.y+1));
		surface->applyImpulseAt(glm::vec3(0,-m_velocity.y/4.f*SURFACE_IMPULSE_COEFFICIENT,0),
								glm::vec3(closestImpulseLocation.x-1, 0, closestImpulseLocation.y-1));

		glm::vec3 vxz = glm::vec3(m_velocity.x, 0, m_velocity.z);
		applyImpulseAt(-SURFACE_SIDEWAYS_COEFFICIENT * m_mass * glm::pow(1-m_center.y/m_radius, 3.f) * vxz, m_center);

		if(m_center.y < 0)
			this->applyForceAt(-GRAVITY * m_mass * (1-m_center.y) * supportingVector, m_center);
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
	m_center.x = glm::clamp(m_center.x, -.5f+m_radius, .5f-m_radius);
	m_center.z = glm::clamp(m_center.z, -.5f+m_radius, .5f-m_radius);
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
