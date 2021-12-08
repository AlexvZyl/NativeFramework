#include "ManagedEntity.h"
#include "../GUI/GUIState.h"
#include "EntityManager.h"


ManagedEntity::ManagedEntity(EntityType type, ManagedEntity* parent)
{
	m_entityID = EntityManager::generateEID(this);
	m_parent = parent;
	m_type = type;
}


void ManagedEntity::setContext(GUIState* guiState)
{
}

void ManagedEntity::setEntityID(unsigned int eID)
{
	m_entityID = eID;
}
