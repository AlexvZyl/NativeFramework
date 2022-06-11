#pragma once

/*
All entities have to be a child of this class.  The data contained in this class is all that is required 
for a VAO to be able to render the entity to the screen.
*/

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "OpenGL/Primitives/IPrimitive.h"
#include "OpenGL/Primitives/Vertex.h"
#include "OpenGL/Buffers/GraphicsPrimitivesBuffersGL.h"
#include "Utilities/Assert/Assert.h"

#include "glm/gtx/transform.hpp"

#include <vector>
#include <memory>

//=============================================================================================================================================//
//  Primitive Class.																														   //
//=============================================================================================================================================//

template<typename BufferType>
class Primitive: public IPrimitive
{
protected:

	// Template types.
	typedef Primitive<BufferType> This;
	typedef BufferType::t_vertexType VertexType;
	typedef BufferType::t_indexType IndexType;

public:

	// Constructor.
	inline Primitive(Entity* parent = nullptr) : IPrimitive(parent) { }

	// Destructor.
	inline virtual ~Primitive() 
	{ 
		if (m_onGPU) removeFromGraphicsBuffer(); 
	}

	virtual void translate(const glm::vec3& translation) 
	{
		for (int i = 0; i < m_vertexCount; i++)
			getVertex(i).data.position += translation;

		m_trackedCenter += translation;
		syncWithGPU();
	}

	virtual void translate(const glm::vec2& translation) 
	{
		This::translate(glm::vec3{ translation, 0.f });
	}

	virtual void translateTo(const glm::vec3& position) 
	{
		This::translate(position - m_trackedCenter);
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

		for (int i = 0; i < m_vertexCount; i++)
			getVertex(i).data.position = glm::vec3(transform * glm::vec4(getVertex(i).data.position, 1.f));

		syncWithGPU();
	}

	virtual void rotate(float degrees, const glm::vec3& rotateNormal = { 0.f, 0.f, 1. }) 
	{
		This::rotate(degrees, m_trackedCenter, rotateNormal);
	}

	virtual void transform(const glm::mat4& transform) 
	{
		for (int i = 0; i < m_vertexCount; i++)
			getVertex(i).data.position = glm::vec3(transform * glm::vec4(getVertex(i).data.position, 1.f));

		// Not sure if this is correct (scaling?).
		m_trackedCenter = glm::vec3(transform * glm::vec4(m_trackedCenter, 1.f));

		syncWithGPU();
	}

	virtual void scale(const glm::vec3& scaling) 
	{
		// ...

		syncWithGPU();
	}

	virtual void enableOutline(float value = 1.f) 
	{
		if (m_outlineEnabled) return;
		m_outlineEnabled = true;
		m_outlineValue = value;

		for (int i = 0; i < m_vertexCount; i++)
			getVertex(i).data.outline = value;

		syncWithGPU();
	}

	virtual void disableOutline() 
	{
		if (!m_outlineEnabled) return;
		m_outlineEnabled = false;
		m_outlineValue = 0.f;

		for (int i = 0; i < m_vertexCount; i++)
			getVertex(i).data.outline = 0.f;

		syncWithGPU();
	}

	// ------------- //
	//  V E R T E X  //
	// ------------- //

	// Get the vertex belonging to this primitive (based on local index).
	inline VertexType& getVertex(int index) 
	{
		LUMEN_DEBUG_ASSERT(index >= 0 && index < m_vertexCount, "Indexing out of range.");
		return getGraphicsBuffer().getVertex(m_vertexBufferPos + index);
	}

	inline virtual void translateVertexTo(VertexType* vertex, const glm::vec3 position) 
	{
		vertex->data.position = position;
		syncWithGPU();
	}

	inline virtual void translateVertexTo(VertexType* vertex, const glm::vec2 position)
	{
		This::translateVertexTo(vertex, glm::vec3{ position, vertex->data.position.z });
	}

	inline virtual void translateVertex(VertexType* vertex, const glm::vec3 translation) 
	{
		vertex->data.position += translation;
		syncWithGPU();
	}

	inline virtual void translateVertex(VertexType* vertex, const glm::vec2 translation) 
	{
		This::translateVertex(vertex, glm::vec3{ translation, 0.f });
	}

	inline virtual void translateVertexAtIndex(unsigned index, const glm::vec3& translation)
	{
		This::translateVertex(&getVertex(index), translation);
	}

	inline virtual void translateVertexAtIndex(unsigned index, const glm::vec2& translation)
	{
		This::translateVertexAtIndex(index, glm::vec3{ translation, 0.f });
	}

	inline virtual void translateVertexAtIndexTo(unsigned index, const glm::vec3& position)
	{
		This::translateVertexTo(&getVertex(index), position);
	}

	inline virtual void translateVertexAtIndexTo(unsigned index, const glm::vec2& position)
	{
		This::translateVertexAtIndexTo(index, glm::vec3{ position, 0.f });
	}

	inline virtual void setVertexAtIndexColor(unsigned index, const glm::vec4& color)
	{
		getVertex(index).data.color = color;
		syncWithGPU();
	}

	inline virtual std::tuple<VertexType&, float> getNearestVertex(const glm::vec3& position)
	{
		auto [index, distance] = getNearestVertexIndex(position);
		return { getVertex(index), distance };
	}

	inline virtual std::tuple<unsigned, float> getNearestVertexIndex(const glm::vec3& position)
	{
		// Calculate the first vertex' distance.
		int nearestVertexIndex = 0;
		VertexType& closestVertex = getVertex(0);;
		float minDistance = glm::abs(glm::distance(position, closestVertex.data.position));
		// Find if any of the vertices are closer.
		for (int i = 0; i < m_vertexCount; i++)
		{
			VertexType& currentVertex = getVertex(i);
			float currentDistance = glm::abs(glm::distance(position, currentVertex.data.position));
			if (currentDistance > minDistance) continue;
			closestVertex = currentVertex;
			nearestVertexIndex = i - m_vertexBufferPos;
			minDistance = currentDistance;
		}
		// Return the closest vertex, alongside the distance in world coordinates.
		return { nearestVertexIndex, minDistance };
	}

	inline virtual std::tuple<VertexType&, float> getNearestVertex(const glm::vec2& position)
	{
		auto [index, distance] = getNearestVertexIndex(position);
		return { getVertex(index), distance };
	}

	inline virtual std::tuple<unsigned, float> getNearestVertexIndex(const glm::vec2& position)
	{
		// Calculate the first vertex' distance.
		int nearestVertexIndex = 0;
		VertexType& closestVertex = getVertex(0);
		float minDistance = glm::abs(glm::distance(position, glm::vec2(closestVertex.data.position)));
		// Find if any of the vertices are closer.
		for (int i = 0; i < m_vertexCount; i++)
		{
			VertexType& currentVertex = getVertex(i);
			float currentDistance = glm::abs(glm::distance(position, glm::vec2(currentVertex.data.position)));
			if (currentDistance > minDistance) continue;
			closestVertex = currentVertex;
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
		for (int i = 0; i < m_vertexCount; i++)
			getVertex(i).data.color = color;

		m_colour = color;
		syncWithGPU();
	}

	virtual void setEntityID(unsigned eID) 
	{
		for (int i = 0; i < m_vertexCount; i++)
			getVertex(i).entityID = eID;

		m_entityID = eID;
		syncWithGPU();
	}

	virtual void setLayer(float layer) 
	{
		for (int i = 0; i < m_vertexCount; i++)
			getVertex(i).data.position.z = layer;

		m_trackedCenter.z = layer;
		syncWithGPU();
	}	

	// ------------- //
	//  M E M O R Y  //
	// ------------- //

	// Construct the vertices and pass them to the GraphicsBuffer.
	// Do not keep a local copy.
	inline virtual void constructVerticesOnGPU() { };

	// Construct the vertices on the CPU without passing them to the GPU.
	inline virtual void constructVerticesOnCPU() { };

	// Set the GPU context that the primitive renders to.
	inline virtual void setGraphicsBuffer(BufferType* buffer) { m_graphicsBuffer = buffer; }

	// Remove the vertex & index data.
	inline void removeFromGraphicsBuffer() 
	{
		LUMEN_DEBUG_ASSERT(m_onGPU, "Data not on GPU.");

		// Remove from GPU.
		getGraphicsBuffer().erase(m_vertexBufferPos, m_vertexCount, m_indexBufferPos, m_indexCount);
		m_onGPU = false;
		m_indexBufferPos = NULL;
		m_vertexBufferPos = NULL;
		m_queuedForSync = false;
	}

	// Push the vertex and index data to the graphics buffer.
	// Also updates the required metadata.
	inline void pushToGraphicsBuffer(const VertexType* vertices, int vertexCount, const IndexType* indices, int indexCount) 
	{
		LUMEN_DEBUG_ASSERT(!m_onGPU, "Data already on GPU.");

		auto [m_vertexBufferPos, m_indexBufferPos] = getGraphicsBuffer().push(vertices, vertexCount, indices, indexCount);
		m_onGPU = true;
		m_vertexCount = vertexCount;
		m_indexCount = indexCount;
		m_queuedForSync = false;
	}

	// Move the vertex & index data from the Graphics Buffer.
	inline void moveFromGraphicsBuffer() 
	{
		LUMEN_DEBUG_ASSERT(m_onGPU, "Data not on GPU.");

		// Prepare memory.
		m_vertexDataCPU.reserve(m_vertexCount);
		m_indexDataCPU.reserve(m_indexCount);

		// Copy vertices.
		m_vertexDataCPU.insert(
			m_vertexDataCPU.end(), 
			getGraphicsBuffer().getVertexData().begin() + m_vertexBufferPos,
			getGraphicsBuffer().getVertexData().begin() + m_vertexBufferPos + m_vertexCount
		);

		// Copy the indices.
		m_indexDataCPU.insert(
			m_indexDataCPU().end(),
			getGraphicsBuffer().getIndexData().begin() + m_indexBufferPos,
			getGraphicsBuffer().getIndexData().begin() + m_indexBufferPos + m_indexCount
		);

		// Offset the indices to original values.
		if (m_vertexBufferPos) for (auto& ind : m_indexDataCPU) ind -= m_vertexBufferPos;

		removeFromGraphicsBuffer();
	}

	// Move the vertex & index data to the Graphics buffer.
	inline void moveToGraphicsBuffer() 
	{
		LUMEN_DEBUG_ASSERT(!m_onGPU, "Data already on GPU.");

		pushToGraphicsBuffer(m_vertexDataCPU.data(), m_vertexDataCPU.size(), m_indexDataCPU.data(), m_indexDataCPU.size());
		clearCPUData();
	}

	// Notify the GPU that the data has to be updated.
	virtual void syncWithGPU() 
	{
		LUMEN_DEBUG_ASSERT(m_onGPU, "There is no data on the GPU to sync to.");

		if (m_queuedForSync) return;
		getGraphicsBuffer().sync(this);
	}

	// Clear the data on the CPU.
	inline void clearCPUData() 
	{
		m_vertexDataCPU.clear();
		m_vertexDataCPU.shrink_to_fit();
		m_indexDataCPU.clear();
		m_indexDataCPU.shrink_to_fit();
	}

	// Update the indices with a new array.
	inline void updateIndices(IndexType* indices, int indexCount) 
	{
		m_indexBufferPos = getGraphicsBuffer().updateIndices(m_indexBufferPos, m_indexCount, m_vertexBufferPos, indices, indexCount);
		m_indexCount = indexCount;
	}

protected:

	// The GPB that the Primitive is contained in (if any).
	BufferType* m_graphicsBuffer = nullptr;
	inline BufferType& getGraphicsBuffer() { return *m_graphicsBuffer; }

	// Local data.
	// This is used when the primitive is constructed outside the context of a Graphics Buffer,
	// of when the data has to be removed from the Graphics buffer but still has to be kept.
	std::vector<VertexType> m_vertexDataCPU;
	std::vector<IndexType> m_indexDataCPU;
	// Is the data on the GPU?
	bool m_onGPU = true;	
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
