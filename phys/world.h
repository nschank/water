#ifndef WORLD_H
#define WORLD_H

#include "entity.h"
#include <vector>

class World
{
public:
	World();
	virtual ~World();

	//Does not take possession of the entity
	void addEntity(Entity *newEntity);

	void tick(float secondsSinceLastTick);

private:
	std::vector<Entity *> m_entities;

};

#endif // WORLD_H
