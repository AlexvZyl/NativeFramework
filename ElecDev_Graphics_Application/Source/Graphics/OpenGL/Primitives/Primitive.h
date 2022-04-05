#pragma once

/*
All entities have to be a child of this class.  The data contained in this class is all that is required 
for a VAO to be able to render the entity to the screen.
*/

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <vector>
#include <memory>
#include "glm/glm.hpp"
#include "Graphics/Entities/Entity.h"

//=============================================================================================================================================//
//  Forward declerations.																													   //
//=============================================================================================================================================//

template<typename VertexType>
class VertexArrayObject;

struct GUIState;

//=============================================================================================================================================//
//  Primitive Pointer.																														   //
//=============================================================================================================================================//

class PrimitivePtr : public Entity
{
public:

	unsigned m_vertexCount = 0;			// Counts the amount of vertices.
	unsigned m_indexCount = 0;			// Counts the amount of indices.
	unsigned m_vertexBufferPos = 0;		// The start position of the entity in the VAO.
	unsigned m_indexBufferPos = 0;		// The position in the indices buffer.
	unsigned m_primitiveBufferPos = 0;	// The primitive position in the VAO buffer.
	bool	 m_queuedForSync = false;	// Has the primitive been synced?

	// Destructor.
	virtual ~PrimitivePtr() = default;

protected:

	// Constructor.
	PrimitivePtr(Entity* parent) 
		: Entity(EntityType::PRIMITIVE, parent) 
	{}
};

//=============================================================================================================================================//
//  Primitive Class.																														   //
//=============================================================================================================================================//

template<typename VertexType>
class Primitive: public PrimitivePtr
{
public:

	// ------------------- //
	//  V A R I A B L E S  //
	// ------------------- //

	VertexArrayObject<VertexType>* m_VAO = nullptr;	// Pointer to the VAO that the entity is drawn to.
	glm::vec4 m_colour = {0.f, 0.f, 0.f, 1.f};		// Saves the global color for the entity.
	glm::vec3 m_trackedCenter = {0.f,0.f,0.f};		// Gives the option to track the center of the entity.
													// Useful for rotation, scaling and moving to a point.
	VertexType* m_vertices;							// Pointer to the first vertex in memory.

	// ------------------------------------------------- //
	//  C O N S T R U C T O R   &   D E S T R U C T O R  //
	// ------------------------------------------------- //

	// Constructor that sets the context as well.
	Primitive(Entity* parent);
	// Destructor.
	~Primitive();

	// ------------------- //
	//  P R I M I T I V E  //
	// ------------------- //

	// Translate the entity by the given vector.
	virtual void translate(const glm::vec3& translation);
	// Translate the entity by the given vector.
	virtual void translate(const glm::vec2& translation);
	// Translate the entity to the given position.
	virtual void translateTo(const glm::vec3& position);
	//Translates the entity in the XY plane, keeping the same Z value.
	virtual void translateTo(const glm::vec2& position);
	// Rotates the entity by the given vector and center point.
	virtual void rotate(float degrees, const glm::vec3& rotateNormal = {0.f, 0.f, 1.});
	// Rotates the entity by the given vector and provided point.
	virtual void rotate(float degrees, const glm::vec3& rotatePoint, const glm::vec3& rotateNormal = { 0.f, 0.f, 1. });
	// Scales the entity by the given vector and center point.
	virtual void scale(const glm::vec3& scaling);
	// Outline the primitive by the given scale.
	virtual void enableOutline();
	// Remove the outline.
	virtual void disableOutline();

	// ------------- //
	//  V E R T E X  //
	// ------------- //

	// Translation.
	virtual void translateVertexAtIndex(unsigned index, const glm::vec3& translation);
	virtual void translateVertexAtIndex(unsigned index, const glm::vec2& translation);
	// Translate to.
	virtual void translateToVertexAtIndex(unsigned index, const glm::vec3& position);
	virtual void translateToVertexAtIndex(unsigned index, const glm::vec2& position);
	// Set color.
	virtual void setVertexColorAtIndex(unsigned index, const glm::vec4& color);
	// Getting nearest vertex.
	virtual std::tuple<VertexType*, float> getNearestVertex(const glm::vec3& position);
	virtual std::tuple<VertexType*, float> getNearestVertex(const glm::vec2& position);
	
	// --------------------- //
	//  A T T R I B U T E S  //
	// --------------------- //

	// Sets the color for all of the vertices.
	virtual void setColor(const glm::vec4& color);
	// Sets the entty ID of the entity.
	virtual void setEntityID(unsigned int eID);
	// Set the entity later.
	virtual void setLayer(float layer);
	// Set context.
	virtual void setContext(GUIState* guiState);

	// ------------- //
	//  M E M O R Y  //
	// ------------- //

	// Deletes the primitive from the GPU but keeps it in the CPU.
	virtual void wipeGPU();
	// Sync the primitive data to the GPU.
	virtual void syncWithGPU();
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//