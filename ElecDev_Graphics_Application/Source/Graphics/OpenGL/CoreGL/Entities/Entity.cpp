#include "Entity.h"
#include "../GUI/GUIState.h"
#include "EntityManager.h"


Entity::Entity(EntityType type, Entity* parent)
{
	m_entityID = EntityManager::generateEID(this);
	m_parent = parent;
	m_type = type;
}

Entity::~Entity()
{
	EntityManager::freeEID(m_entityID);
}


void Entity::setContext(GUIState* guiState)
{
}
