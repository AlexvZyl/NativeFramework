#pragma once
#include <vector>

class EntityManager
{
private:
	//Store the last ID used
	static unsigned lastID;
	//Vector to store any IDs that have been freed before reuse
	static std::vector<unsigned> freeIDs;

public:
	//Generate a new EID
	static unsigned generateEID();
	//free a specified EID to be reused
	static void freeEID(unsigned EID);

};



