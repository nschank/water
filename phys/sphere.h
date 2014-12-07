#ifndef SPHERE_H
#define SPHERE_H

#include "entity.h"

class Sphere : public Entity
{
public:
	Sphere();
	virtual ~Sphere();

	Collision *collisionWith(Entity *other);

protected:
	Collision *collisionWithSphere(Sphere *other);

	glm::vec3 m_center;
	float m_radius;
};

#endif // SPHERE_H
