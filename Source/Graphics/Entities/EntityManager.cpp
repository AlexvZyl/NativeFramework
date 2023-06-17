//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Utilities/Logger/Logger.h"
#include "EntityManager.h"

//==============================================================================================================================================//
//  Methods.																																	//
//==============================================================================================================================================//

void EntityManager::init() 
{
	// Reserve slot 0 for `non` entities.
	s_entityLog.emplace(nullptr, 0);
}

unsigned EntityManager::generateEID(Entity* entity)
{
	return s_entityLog.emplace(entity, 0);
}

void EntityManager::freeEID(unsigned EID)
{	
	s_entityLog.erase(EID);
}

Entity* EntityManager::getEntity(unsigned EID)
{
	// Entity Manager does not handle these values.
	// They are reserved for `non` entities.
	if (EID == -1 || EID == 0) return nullptr;

	// If the slot is used get the entity.
	if (s_entityLog.isSlotUsed(EID))
	{
		return s_entityLog[EID].entity;
	}

	// Slot is not used and invalid entity ID.
	else
	{
		LUMEN_LOG_WARN("Invalid entity ID.", "Entity Manager");
		return nullptr;
	}
}

unsigned EntityManager::peakNextID() 
{
	// If the Freelist is full, the next ID will be the capacity.
	// Otherwise, the FreeList will always place it in the first open slot.

	int firstFreeSlot = s_entityLog.getFirstFreeSlot();
	// FreeList is not full.
	if (firstFreeSlot != -1) return firstFreeSlot;
	// Capacity is the next slot.
	else return s_entityLog.capacity();
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//

