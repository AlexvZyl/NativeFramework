#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Utilities/Memory/FreeList.h"

#include <vector>
#include <unordered_map>

//==============================================================================================================================================//
//  Forward declerations.																														//
//==============================================================================================================================================//

class Entity;

struct EntityContainer 
{
	// Entity logged in this container.
	Entity* entity;

	// Placeholder data so that we can use FreeLists.
	// SEFreeLists need 64 bits (8 bytes).
	int placeHolder;
};

//==============================================================================================================================================//
//  EntityManager Class.																														//
//==============================================================================================================================================//

class EntityManager
{

public:


	// Setup the manager resources.
	static void init();

	// Generate a new entity ID.
	static unsigned generateEID(Entity* entity);

	// Free a specified EID to be reused.
	static void freeEID(unsigned EID);

	// Get the entity given the ID.
	static Entity* getEntity(unsigned EID);

	// Returns the ID that is going to be assigned next.
	static unsigned peakNextID();

private:

	// Store entities logged by Lumen.
	inline static SEFreeList<EntityContainer> s_entityLog;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//