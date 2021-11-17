#include "EntityManager.h"
#include <iostream>


unsigned EntityManager::lastID = 0;
std::vector<unsigned> EntityManager::freeIDs;
std::vector<ManagedEntity*> EntityManager::entityLog;

unsigned EntityManager::generateEID(ManagedEntity* entity)
{
	//Check to see if there are any freed (recycled) IDs
	if (!freeIDs.size()) {
		entityLog.push_back(entity);
		return ++lastID;
	}
	else {//recycle IDs
		unsigned freeID = freeIDs.back();
		freeIDs.pop_back();
		entityLog[freeID - 1] = entity;
		return freeID;
	}
}

void EntityManager::freeEID(unsigned EID)
{	
	//To free the last EID, we can simply decrement LastID
	if (EID == lastID) {
		lastID--;
	}
	else {//we need to remember to recyce this ID
		freeIDs.push_back(EID);
	}
}

ManagedEntity* EntityManager::getEntity(unsigned EID)
{
	if ((EID == 0) || (EID == -1)) {
		std::cout << "\nEntities with ID = -1 or 0 are not managed by the entity manager.";
		return nullptr;
	}
	if (entityLog.size() >= EID) {
		return entityLog[EID - 1];
	}
	else {
		std::cout << "\nInvalid eID.";
		return nullptr;
	}
}
