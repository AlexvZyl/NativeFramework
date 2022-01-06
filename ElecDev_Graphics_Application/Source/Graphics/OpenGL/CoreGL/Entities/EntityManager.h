#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <vector>

//==============================================================================================================================================//
//  Forward declerations.																														//
//==============================================================================================================================================//

class Entity;

//==============================================================================================================================================//
//  EntityManager Class.																														//
//==============================================================================================================================================//

class EntityManager
{

public:

	// Generate a new entity ID.
	static unsigned generateEID(Entity* entity);
	// Free a specified EID to be reused.
	static void freeEID(unsigned EID);
	// Get the entity given the ID.
	static Entity* getEntity(unsigned EID);
	// Returns the last ID assigned.
	static unsigned getLastID();
	// Returns the ID that is going to be assigned next.
	// TODO: unsigned peakNextID();

private:

	// Store the last ID used
	static unsigned lastID;
	//Vector to store any IDs that have been freed before reuse
	static std::vector<unsigned> freeIDs;
	//Vector to store pointers to each registered entity
	static std::vector<Entity*> entityLog;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//