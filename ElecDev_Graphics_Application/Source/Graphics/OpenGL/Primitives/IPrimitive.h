#pragma once

#include "Graphics/Entities/Entity.h"
#include "glm/glm.hpp"

class IPrimitive : public Entity
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

	// Destructor.
	virtual ~IPrimitive() = default;

protected:

	friend class Renderer;
	inline virtual void removeFromGraphicsBuffer() = 0;

	// Constructor.
	IPrimitive(Entity* parent) : Entity(EntityType::PRIMITIVE, parent) { }
}; 
