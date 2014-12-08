#include "watersurface.h"
#include "sphere.h"

//STENCIL

WaterSurface::WaterSurface()
{

}

WaterSurface::~WaterSurface()
{

}

void WaterSurface::applyTranslationAt(glm::vec3 translation, glm::vec3 location)
{

}

void WaterSurface::applyImpulseAt(glm::vec3 impulse, glm::vec3 location)
{

}

void WaterSurface::applyForceAt(glm::vec3 force, glm::vec3 location)
{
//Unused
}

void WaterSurface::tick(float secondsSinceLastTick)
{

}

Collision *WaterSurface::collisionWith(Entity *other)
{
	return other->collisionWithSurface(this);
}


Collision *WaterSurface::collisionWithSphere(Sphere *other)
{
	return other->collisionWithSurface(this);
}

Collision *WaterSurface::collisionWithSurface(WaterSurface *other)
{
	//No thank you
	return NULL;
}

