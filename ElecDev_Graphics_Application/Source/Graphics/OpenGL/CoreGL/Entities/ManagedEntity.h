#pragma once
#include "GUI/GUIState.h"
class ManagedEntity
{
private:

public:
	ManagedEntity* m_parent = nullptr;							// Parent of the managed entity
	unsigned int m_entityID = 0;						// Saves the global entity ID.

	ManagedEntity();
	ManagedEntity(ManagedEntity* parent);

	// Set the Context of this entity, and parents.
	virtual void setContext(GUIState* guiState);
	// Sets the entty ID of the entity.
	virtual void setEntityID(unsigned int eID);
};

