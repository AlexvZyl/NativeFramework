#pragma once

#include "Graphics/Entities/Entity.h"
#include "glm/glm.hpp"
#include "Engines/EntityComponents/Mutable.h"

class IPrimitive : public Entity, public Translatable, public Rotatable, public Transformable, public Scalable, public Reshapable
{
public:

	// Data.
	int m_vertexCount = 0;							// Counts the amount of vertices.
	int m_indexCount = 0;							// Counts the amount of indices.
	int m_vertexBufferPos = -1;						// The start position of the primitive vertices in the GPB.
	int m_indexBufferPos = -1;						// The start position of the primitive indices in the GPB.
	glm::vec4 m_colour = { 0.f, 0.f, 0.f, 1.f };	// Saves the global color for the primitive.
	glm::vec3 m_trackedCenter = { 0.f,0.f,0.f };	// Gives the option to track the center of the primitive.
	bool m_queuedForSync = false;					// Is the primitive going to be synced on the next draw call.
	bool m_outlineEnabled = false;					// Does the primitive has its outline enabled.
	float m_outlineValue = 0.f;						// The alpha value (or bloom intensity?) of the outline.

	// API.	
	// Not quite sure if we need this.
	inline virtual void setColor(const glm::vec4& color) = 0;
	inline virtual void setEntityID(unsigned int eID) = 0;
	inline virtual void setLayer(float layer) = 0; 
	virtual void disableOutline() = 0;
	virtual void enableOutline(float value = 1.f) = 0;
	// Translatable
	virtual void translate(const glm::vec2& translation) = 0;
	virtual void translateTo(const glm::vec2& position) = 0;

	virtual void translate(const glm::vec3& translation) = 0;
	virtual void translateTo(const glm::vec3& position) = 0;

	// Rotatable
	virtual void rotate(float degrees, const glm::vec3& rotateNormal = { 0.f, 0.f, 1.f }) = 0;

	//Scalable
	virtual void scale(const glm::vec3& scaleFactor) = 0;

	// Reshapable
	virtual void translateVertexAtIndex(const unsigned& localIndex, const glm::vec2& translation) override = 0;
	virtual void translateVertexAtIndexTo(const unsigned& localIndex, const glm::vec2& position) override = 0;
	virtual unsigned logicalVertexCount() override = 0;
	virtual glm::vec2 getLogicalVertex(unsigned localIndex) = 0;

	// Destructor.
	virtual ~IPrimitive() = default;

protected:

	// Constructor.
	IPrimitive(Entity* parent) : Entity(EntityType::PRIMITIVE, parent) { }
}; 
