#ifndef WATERSURFACE_H
#define WATERSURFACE_H

#include "entity.h"

class WaterSurface : public Entity
{
friend class Sphere;

public:
	WaterSurface();
	virtual ~WaterSurface();

	void collideWith(Entity *other);

	void applyTranslationAt(glm::vec3 translation, glm::vec3 location);
	void applyImpulseAt(glm::vec3 impulse, glm::vec3 location);
	void applyForceAt(glm::vec3 force, glm::vec3 location);
	void tick(float secondsSinceLastTick);

protected:
	void collideWithSphere(Sphere *other);
	void collideWithSurface(WaterSurface *other);

	float heightAt(float x, float y);
	float getXResolution();
	float getYResolution();

	float getMaxHeight();
};

#endif // WATERSURFACE_H
