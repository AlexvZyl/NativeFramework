#pragma once
#include <vector>
class Entity;

class EntityManager
{
private:

	//Store the last ID used
	static unsigned lastID;
	//Vector to store any IDs that have been freed before reuse
	static std::vector<unsigned> freeIDs;
	//Vector to store pointers to each registered entity
	static std::vector<Entity*> entityLog;

public:

	//Generate a new EID
	static unsigned generateEID(Entity* entity);
	//free a specified EID to be reused
	static void freeEID(unsigned EID);
	static Entity* getEntity(unsigned EID);
};