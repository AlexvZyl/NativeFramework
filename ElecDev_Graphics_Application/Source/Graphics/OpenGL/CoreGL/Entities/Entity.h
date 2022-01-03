#pragma once
#include "GUI/GUIState.h"

enum class EntityType
{
	PRIMITIVE,
	PORT,
	COMPONENT,
	CIRCUIT,
	CABLE
};

class Entity
{
private:

public:
	Entity* m_parent = nullptr;		// Parent of the managed entity
	unsigned int m_entityID = 0;	// Saves the global entity ID.
	EntityType m_type;

	//Entity();
	Entity(EntityType type, Entity* parent = nullptr);
	~Entity();

	// Set the Context of this entity, and parents.
	virtual void setContext(GUIState* guiState);
};

