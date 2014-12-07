#include "sphere.h"

Sphere::Sphere()
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
