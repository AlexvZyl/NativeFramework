#pragma once
#include "GUI/GUIState.h"
enum class EntityType
{
	PRIMATIVE,
	PORT,
	COMPONENT,
	CIRCUIT
};


class ManagedEntity
{
private:

public:
	ManagedEntity* m_parent = nullptr;							// Parent of the managed entity
	unsigned int m_entityID = 0;								// Saves the global entity ID.
	EntityType m_type;

	//ManagedEntity();
	ManagedEntity(EntityType type, ManagedEntity* parent = nullptr);

	// Set the Context of this entity, and parents.
	virtual void setContext(GUIState* guiState);
	// Sets the entty ID of the entity.
	virtual void setEntityID(unsigned int eID);
};

