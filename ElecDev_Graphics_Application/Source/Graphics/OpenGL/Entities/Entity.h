#pragma once

//==============================================================================================================================================//
//  Types.																																	    //
//==============================================================================================================================================//

enum class EntityType
{
	PRIMITIVE,
	PORT,
	COMPONENT,
	CIRCUIT,
	CABLE
};

//==============================================================================================================================================//
//  Entity Class.																																//
//==============================================================================================================================================//

class Entity
{
private:

public:

	Entity* m_parent = nullptr;		// Parent of the managed entity
	unsigned int m_entityID = 0;	// Saves the global entity ID.
	EntityType m_type;

	// Constructor.
	Entity(EntityType type, Entity* parent = nullptr);
	// Destructor.
	virtual ~Entity();

	// Set the Context of this entity, and parents.
	virtual void setContext();
};

//==============================================================================================================================================//
//  EOF.																																	    //
//==============================================================================================================================================//