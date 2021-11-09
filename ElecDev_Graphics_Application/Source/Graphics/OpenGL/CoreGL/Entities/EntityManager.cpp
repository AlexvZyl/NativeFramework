#include "EntityManager.h"

unsigned EntityManager::generateEID()
{
	//Check to see if there are any freed (recycled) IDs
	if (!freeIDs.size()) {
		return ++lastID;
	}
	else {//recycle IDs
		unsigned freeID = freeIDs.back();
		freeIDs.pop_back();
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
