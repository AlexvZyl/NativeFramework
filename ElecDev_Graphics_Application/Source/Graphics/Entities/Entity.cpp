//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Entity.h"
#include "EntityManager.h"
#include "Utilities/Logger/Logger.h"

//==============================================================================================================================================//
//  Methods.																																	//
//==============================================================================================================================================//

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

void Entity::transferTo(Entity* newOwner)
{
	transferTo(newOwner->m_entityID);
}

void Entity::transferTo(unsigned newOwnerID)
{
	//Check that we are not transferring to self
	if (newOwnerID != m_entityID) {
		EntityManager::freeEID(m_entityID);
		m_entityID = newOwnerID;
	}
	else {
		LUMEN_LOG_WARN("Cannot transfer ownership to self.", "Entity");
	}
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
