#include "ManagedEntity.h"
#include "../GUI/GUIState.h"
#include "EntityManager.h"


ManagedEntity::ManagedEntity()
{
	m_entityID = EntityManager::generateEID(this);
}

ManagedEntity::ManagedEntity(ManagedEntity* parent) : ManagedEntity()
{
	m_parent = parent;
}


void ManagedEntity::setContext(GUIState* guiState)
{
}

void ManagedEntity::setEntityID(unsigned int eID)
{
	m_entityID = eID;
}
