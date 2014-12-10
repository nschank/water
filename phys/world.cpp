#include "world.h"

World::World()
{

}

World::~World()
{

}

void World::addEntity(Entity *newEntity)
{
	m_entities.push_back(newEntity);
}

void World::tick(float secondsSinceLastTick)
{


	for(int i = 0; i < m_entities.size(); i++)
	{
		Entity *A = m_entities[i];
		for(int j = i+1; j < m_entities.size(); j++)
		{
			Entity *B = m_entities[j];
			A->collideWith(B);
		}
	}

	for(std::vector<Entity *>::const_iterator it = m_entities.begin(); it != m_entities.end(); it++)
		(*it)->tick(secondsSinceLastTick);

}
