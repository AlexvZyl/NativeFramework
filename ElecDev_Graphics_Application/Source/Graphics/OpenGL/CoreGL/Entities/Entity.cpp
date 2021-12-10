#include "Entity.h"
#include "../GUI/GUIState.h"
#include "EntityManager.h"


Entity::Entity(EntityType type, Entity* parent)
{
	m_entityID = EntityManager::generateEID(this);
	m_parent = parent;
	m_type = type;
}


void Entity::setContext(GUIState* guiState)
{
}

void Entity::setEntityID(unsigned int eID)
{
	m_entityID = eID;
}
