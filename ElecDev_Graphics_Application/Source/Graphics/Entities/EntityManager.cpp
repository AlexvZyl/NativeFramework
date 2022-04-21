//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "EntityManager.h"
#include <iostream>
#include "Utilities/Logger/Logger.h"

//==============================================================================================================================================//
//  Static inits.																																//
//==============================================================================================================================================//

unsigned EntityManager::lastID = 0;
std::vector<unsigned> EntityManager::freeIDs;
std::unordered_map<unsigned, Entity*> EntityManager::entityLog = std::unordered_map<unsigned, Entity*>();

//==============================================================================================================================================//
//  Methods.																																	//
//==============================================================================================================================================//

unsigned EntityManager::generateEID(Entity* entity)
{
	// Check to see if there are any freed (recycled) ID's.
	if (!freeIDs.size())  
	{
		entityLog.insert({ ++lastID, entity });
		return lastID;
	}
	else  // Recycle ID's.
	{
		unsigned freeID = freeIDs.back();
		freeIDs.pop_back();
		entityLog.insert({ freeID, entity });
		return freeID;
	}
}

void EntityManager::freeEID(unsigned EID)
{	
	// To free the last EID, we can simply decrement LastID
	if (EID == lastID) 
	{ 
		lastID--; 
	}
	// Remember to recycle this ID.
	else			   
	{ 
		freeIDs.push_back(EID); 
		// Consider invalidating pointers to deleted entities in the log here.
	}
	entityLog.erase(EID);
}

Entity* EntityManager::getEntity(unsigned EID)
{
	if ((EID == 0) || (EID == -1)) 
	{
		LUMEN_LOG_WARN("Entities with ID = -1 or 0 are not managed by the entity manager.", "Entity Manager");
		return nullptr;
	}
	if (entityLog.contains(EID))
	{
		return entityLog.at(EID);
	}
	else 
	{
		LUMEN_LOG_WARN("Invalid entity ID.", "Entity Manager");
		return nullptr;
	}
}

unsigned EntityManager::getLastID() 
{ 
	return lastID; 
}

unsigned EntityManager::peakNextID() 
{
	// Check to see if there are any freed (recycled) ID's.
	if (!freeIDs.size())
	{
		return lastID+1;
	}
	// Latest free ID.
	else  
	{
		return freeIDs.back();
	}
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//

