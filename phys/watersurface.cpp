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

void WaterSurface::collideWith(Entity *other)
{
	other->collideWithSurface(this);
}


void WaterSurface::collideWithSphere(Sphere *other)
{
	other->collideWithSurface(this);
}

void WaterSurface::collideWithSurface(WaterSurface *other)
{
	//does nothing
}

float WaterSurface::heightAt(float x, float y)
{
	return 0;
}

float WaterSurface::getXResolution()
{
	return .001;
}

float WaterSurface::getYResolution()
{
	return .001;
}

float WaterSurface::getMaxHeight()
{
	return .5;
}

