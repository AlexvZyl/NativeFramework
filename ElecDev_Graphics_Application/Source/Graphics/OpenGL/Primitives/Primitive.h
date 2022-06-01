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
//  Primitive Pointer.																														   //
//=============================================================================================================================================//

class IPrimitive : public Entity
{
public:

	// Data.
	unsigned m_vertexCount = 0;						// Counts the amount of vertices.
	unsigned m_indexCount = 0;						// Counts the amount of indices.
	unsigned m_vertexBufferPos = 0;					// The start position of the primitive vertices in the GPB.
	unsigned m_indexBufferPos = 0;					// The start position of the primitive indices in the GPB.
	glm::vec4 m_colour = { 0.f, 0.f, 0.f, 1.f };	// Saves the global color for the primitive.
	glm::vec3 m_trackedCenter = { 0.f,0.f,0.f };	// Gives the option to track the center of the primitive.
	bool m_outlineEnabled = false;

	// API.	
	inline virtual void setColor(const glm::vec4& color) = 0;
	inline virtual void setEntityID(unsigned int eID) = 0;
	inline virtual void setLayer(float layer) = 0;

	// Destructor.
	virtual ~IPrimitive() = default;

protected:

	// Constructor.
	IPrimitive(Entity* parent) : Entity(EntityType::PRIMITIVE, parent) { }
};

//=============================================================================================================================================//
//  Primitive Class.																														   //
//=============================================================================================================================================//

template<typename BufferType>
class Primitive: public IPrimitive
{
	constexpr typedef Primitive<BufferType> This;
	constexpr typedef BufferType::t_vertexType VertexType;
	constexpr typedef BufferType::t_indexType IndexType;

public:

	// Constructors.
	inline Primitive(Entity* parent = nullptr) : IPrimitive(parent) { }

	// Destructor.
	inline virtual ~Primitive() { if (m_onGPU) removeFromGraphicsBuffer(); }

	// ------------------- //
	//  P R I M I T I V E  //
	// ------------------- //

	virtual void translate(const glm::vec3& translation) 
	{
		for (int i = m_vertexBufferPos; i < m_vertexBufferPos + m_vertexCount; i++)
			getGraphicsBuffer().getVertex(i).data.position += translation;

		m_trackedCenter += translation;
		syncWithGPU();
	}

	virtual void translate(const glm::vec2& translation) 
	{
		This::translate(glm::vec3{ translation, 0.f });
	}

	virtual void translateTo(const glm::vec3& position) 
	{
		glm::vec3 translation = position - m_trackedCenter;
		for (int i = m_vertexBufferPos; i < m_vertexBufferPos + m_vertexCount; i++)
			getGraphicsBuffer().getVertex(i).data.position += translation;

		m_trackedCenter += translation;
		syncWithGPU();
	}

	virtual void translateTo(const glm::vec2& position) 
	{
		This::translateTo(glm::vec3{ position, m_trackedCenter.z });
	}

	virtual void rotate(float degrees, const glm::vec3& rotatePoint, const glm::vec3& rotateNormal) 
	{
		// Rather call transform if a lot of these are going to be called sequentially.
		glm::mat4 transform = glm::translate(glm::mat4(1.f), rotatePoint);
		transform = glm::rotate(transform, glm::radians(degrees), rotateNormal);
		transform = glm::translate(transform, -rotatePoint);

		for (int i = m_vertexBufferPos; i < m_vertexBufferPos + m_vertexCount; i++)
			getGraphicsBuffer().getVertex(i).data.position = glm::vec3(transform * glm::vec4(getGraphicsBuffer().getVertex(i).data.position, 1.f));

		syncWithGPU();
	}

	virtual void rotate(float degrees, const glm::vec3& rotateNormal = { 0.f, 0.f, 1. }) 
	{
		This::rotate(degrees, m_trackedCenter, rotateNormal);
	}

	virtual void transform(const glm::mat4& transform) 
	{
		for (int i = m_vertexBufferPos; i < m_vertexBufferPos + m_vertexCount; i++)
			getGraphicsBuffer().getVertex(i).data.position = glm::vec3(transfromMatrix * glm::vec4(getGraphicsBuffer().getVertex(i).data.position, 1.f));

		// Not sure if this is correct (scaling?).
		m_trackedCenter = glm::vec3(transfromMatrix * glm::vec4(m_trackedCenter, 1.f));

		syncWithGPU();
	}

	virtual void scale(const glm::vec3& scaling) 
	{
		// ...

		syncWithGPU();
	}

	virtual void enableOutline() 
	{
		if (m_outlineEnabled) return;
		m_outlineEnabled = true;

		for (int i = m_vertexBufferPos; i < m_vertexBufferPos + m_vertexCount; i++)
			getGraphicsBuffer().getVertex(i).data.outline = 1.0f;

		syncWithGPU();
	}

	virtual void disableOutline() 
	{
		if (!m_outlineEnabled) return;
		m_outlineEnabled = false;

		for (int i = m_vertexBufferPos; i < m_vertexBufferPos + m_vertexCount; i++)
			getGraphicsBuffer().getVertex(i).data.outline = 0.f;

		syncWithGPU();
	}

	// ------------- //
	//  V E R T E X  //
	// ------------- //

	inline virtual void translateVertexTo(VertexType* vertex, const glm::vec3 position) 
	{
		vertex->data.position = position;
		syncWithGPU();
	}

	inline virtual void translateVertexTo(VertexType* vertex, const glm::vec2 position) 
	{
		This::translateVertexTo(vertex, glm::vec3(position, vertex->data.position.z));
	}

	inline virtual void translateVertex(VertexType* vertex, const glm::vec3 translation) 
	{
		vertex->data.position += translation;
		syncWithGPU();
	}

	inline virtual void translateVertex(VertexType* vertex, const glm::vec2 translation) 
	{
		This::translateVertex(vertex, glm::vec3(translation, 0.f));
	}

	inline virtual void translateVertexAtIndex(unsigned index, const glm::vec3& translation)
	{
		This::translateVertex(&getGraphicsBuffer().getVertex(m_vertexBufferPos + index), translation);
	}

	inline virtual void translateVertexAtIndex(unsigned index, const glm::vec2& translation)
	{
		This::translateVertexAtIndex(index, { translation.x, translation.y, 0.f });
	}

	inline virtual void translateVertexAtIndexTo(unsigned index, const glm::vec3& position)
	{
		This::translateVertexTo(&getGraphicsBuffer().getVertex(m_vertexBufferPos + index), position);
	}

	inline virtual void translateVertexAtIndexTo(unsigned index, const glm::vec2& position)
	{
		This::translateVertexAtIndexTo(index, { position.x, position.y, 0.f });
	}

	inline virtual void setVertexAtIndexColor(unsigned index, const glm::vec4& color)
	{
		getGraphicsBuffer().getVertex(m_vertexBufferPos + index).data.color = color;
		syncWithGPU();
	}

	inline virtual std::tuple<VertexType*, float> getNearestVertex(const glm::vec3& position)
	{
		auto [index, distance] = getNearestVertexIndex(position);
		return { getGraphicsBuffer().getVertex(index + m_vertexBufferPos), distance };
	}

	inline virtual std::tuple<unsigned, float> getNearestVertexIndex(const glm::vec3& position)
	{
		float minDistance = 0.f;
		VertexType* closestVertex = &getGraphicsBuffer().getVertex(m_vertexBufferPos);;
		// Calculate the first vertex' distance.
		minDistance = glm::abs(glm::distance(position, closestVertex->data.position));
		// Find if any of the vertices are closer.
		int nearestVertexIndex = 0;
		for (int i = m_vertexBufferPos + 1; i < m_vertexBufferPos + m_vertexCount; i++)
		{
			float currentDistance = glm::abs(glm::distance(position, getGraphicsBuffer().getVertex(i).data.position));
			if (currentDistance > minDistance) continue;
			closestVertex = &getGraphicsBuffer().getVertex(i)];
			nearestVertexIndex = i - m_vertexBufferPos;
			minDistance = currentDistance;
		}
		// Return the closest vertex, alongside the distance in world coordinates.
		return { nearestVertexIndex, minDistance };
	}

	inline virtual std::tuple<VertexType*, float> getNearestVertex(const glm::vec2& position)
	{
		auto [index, distance] = getNearestVertexIndex(position);
		return { getGraphicsBuffer().getVertex(index + m_vertexBufferPos), distance };
	}

	inline virtual std::tuple<unsigned, float> getNearestVertexIndex(const glm::vec2& position)
	{
		float minDistance = 0.f;
		VertexType* closestVertex = &getGraphicsBuffer().getVertex(m_vertexBufferPos);;
		// Calculate the first vertex' distance.
		minDistance = glm::abs(glm::distance(position, closestVertex->data.position));
		// Find if any of the vertices are closer.
		int nearestVertexIndex = 0;
		for (int i = m_vertexBufferPos + 1; i < m_vertexBufferPos + m_vertexCount; i++)
		{
			float currentDistance = glm::abs(glm::distance(position, glm::vec2(getGraphicsBuffer().getVertex(i).data.position)));
			if (currentDistance > minDistance) continue;
			closestVertex = &getGraphicsBuffer().getVertex(i)];
			nearestVertexIndex = i - m_vertexBufferPos;
			minDistance = currentDistance;
		}
		// Return the closest vertex, alongside the distance in world coordinates.
		return { nearestVertexIndex, minDistance };
	}
	
	// --------------------- //
	//  A T T R I B U T E S  //
	// --------------------- //

	virtual void setColor(const glm::vec4& color) 
	{
		for (int i = m_vertexBufferPos; i < m_vertexBufferPos + m_vertexCount; i++)
			getGraphicsBuffer().getVertex(i).data.color = color;

		m_colour = color;
		syncWithGPU();
	}

	virtual void setEntityID(unsigned int eID) 
	{
		for (int i = m_vertexBufferPos; i < m_vertexBufferPos + m_vertexCount; i++)
			getGraphicsBuffer().getVertex(i).entityID = eID;

		m_entityID = eID;
		syncWithGPU();
	}

	virtual void setLayer(float layer) 
	{
		for (int i = m_vertexBufferPos; i < m_vertexBufferPos + m_vertexCount; i++)
			getGraphicsBuffer().getVertex(i).data.position.z = layer;

		m_trackedCenter.z = layer;
		syncWithGPU();
	}	

	// ------------- //
	//  M E M O R Y  //
	// ------------- //

	// Construct the vertices and pass them to the GraphicsBuffer.
	// Do not keep a local copy.
	inline virtual void constructVerticesOnGPU() = 0;

	// Construct the vertices locally.
	inline virtual void constructVerticesLocally() = 0;

	// Set the GPU context that the primitive renders to.
	inline virtual void setGraphicsBuffer(BufferType<VertexType>* buffer) { m_graphicsBuffer = buffer; }

	// Remove the vertex & index data.
	inline void removeFromGraphicsBuffer() 
	{
		// Data not on GPU.
		assert(m_onGPU);  
		m_onGPU = false;

		// Remove from GPU.
		getGraphicsBuffer().erase(m_vertexBufferPos, m_vertexCount, m_indexBufferPos, m_indexCount);
		// Reset metadata.
		m_vertexBufferPos = NULL;
		m_indexBufferPos = NULL;
		m_vertexCount = 0;
		m_indexCount = 0;
	}

	// Move the vertex & index data from the Graphics Buffer.
	inline void moveFromGrapicsBuffer() 
	{
		// Data not on GPU.
		assert(m_onGPU);
		m_onGPU = false;
	}

	// Move the vertex & index data to the Graphics buffer.
	inline void moveToGraphicsBuffer() 
	{
		// Data already on GPU.
		assert(!m_onGPU);
		m_onGPU = true;
	}

	// Notify the GPU that the data has to be updated.
	virtual void syncWithGPU() 
	{
		// There is not data on the GPU to sync to.
		assert(m_onGPU);
	}

	// Clear the data on the CPU.
	inline void clearLocalData() 
	{
		// The data is not on the CPU, but on the GPU.
		// This will not break anything, but there is a fault in the logic.
		assert(!m_onGPU);  

		// Clear the CPU data.
		m_localVertexData.clear();
		m_localVertexData.shrink_to_fit();
		m_localIndexData.clear();
		m_localIndexData.shrink_to_fit();
	}

protected:

	// The GPB that the Primitive is contained in (if any).
	BufferType<VertexType>* m_graphicsBuffer = nullptr;
	inline BufferType<VertexType>& getGraphicsBuffer() { return m_graphicsBuffer; }

	// Local data.
	// This is used when the primitive is constructed outside the context of a Graphics Buffer,
	// of when the data has to be removed from the Graphics buffer but still has to be kept.
	std::vector<VertexType> m_localVertexData;
	std::vector<IndexType> m_localIndexData;
	// Is the data on the GPU?
	bool m_onGPU = false;	
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
