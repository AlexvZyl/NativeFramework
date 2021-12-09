#pragma once
#include <vector>
class ManagedEntity;

class EntityManager
{
private:

	//Store the last ID used
	static unsigned lastID;
	//Vector to store any IDs that have been freed before reuse
	static std::vector<unsigned> freeIDs;
	//Vector to store pointers to each registered entity
	static std::vector<ManagedEntity*> entityLog;

public:

	//Generate a new EID
	static unsigned generateEID(ManagedEntity* entity);
	//free a specified EID to be reused
	static void freeEID(unsigned EID);
	static ManagedEntity* getEntity(unsigned EID);
};