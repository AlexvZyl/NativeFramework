//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "EntityManager.h"
#include <iostream>

//==============================================================================================================================================//
//  Static inits.																																//
//==============================================================================================================================================//

unsigned EntityManager::lastID = 0;
std::vector<unsigned> EntityManager::freeIDs;
std::vector<Entity*> EntityManager::entityLog;

//==============================================================================================================================================//
//  Methods.																																	//
//==============================================================================================================================================//

unsigned EntityManager::generateEID(Entity* entity)
{
	// Check to see if there are any freed (recycled) ID's.
	if (!freeIDs.size()) 
	{
		entityLog.push_back(entity);
		return ++lastID;
	}
	else  // Recycle ID's.
	{
		unsigned freeID = freeIDs.back();
		freeIDs.pop_back();
		entityLog[freeID - 1] = entity;
		return freeID;
	}
}

void EntityManager::freeEID(unsigned EID)
{	
	// To free the last EID, we can simply decrement LastID
	if (EID == lastID) 
	{ 
		lastID--; 
		entityLog.pop_back();
	}
	// Remember to recycle this ID.
	else			   
	{ 
		freeIDs.push_back(EID); 
		// Consider invalidating pointers to deleted entities in the log here.
	}
}

Entity* EntityManager::getEntity(unsigned EID)
{
	if ((EID == 0) || (EID == -1)) 
	{
		std::cout << "\nEntities with ID = -1 or 0 are not managed by the entity manager.";
		return nullptr;
	}
	if (entityLog.size() >= EID)
	{
		return entityLog[EID - 1];
	}
	else 
	{
		std::cout << "\nInvalid eID.";
		return nullptr;
	}
}

unsigned EntityManager::getLastID() 
{ 
	return lastID; 
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//

