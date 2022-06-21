#pragma once

#include "Graphics/Entities/Entity.h"
#include "glm/glm.hpp"

class IPrimitive : public Entity
{
public:

	// Public API.	
	inline virtual void setColor(const glm::vec4& color) = 0;
	inline virtual void setEntityID(unsigned int eID) = 0;
	inline virtual void setLayer(float layer) = 0;

	// Getters.
	inline bool isOutlined() const { return m_outlineEnabled; }
	inline int getVertexCount() const { return m_vertexCount; }
	inline int getIndexCount() const { return m_indexCount; }
	inline int getVertexBufferPosition() const { return m_vertexBufferPos; }
	inline int getIndexBufferPosition() const { return m_indexBufferPos; }
	inline bool isQueuedForSync() const { return m_queuedForSync; }
	inline float getOutlineValue() const { return m_outlineValue; }
	inline const glm::vec4& getColor() const { return m_colour; }
	inline const glm::vec3& getTrackedCenter() const { return m_trackedCenter; }
	inline void setQueuedForSync(bool synced) { m_queuedForSync = synced; }

	// Destructor.
	virtual ~IPrimitive() = default;

protected:

	friend class Renderer;

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

	// Protected API.
	inline virtual void removeFromGraphicsBuffer() = 0;

	// Constructor.
	IPrimitive(Entity* parent) : Entity(EntityType::PRIMITIVE, parent) { }
}; 
