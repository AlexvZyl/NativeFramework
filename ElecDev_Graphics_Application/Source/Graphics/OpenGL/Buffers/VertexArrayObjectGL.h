#pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <glad/glad.h>
#include <vector>
#include "glm/glm.hpp"
#include <memory>

//=============================================================================================================================================//
//  Forward declerations.																													   //
//=============================================================================================================================================//

class PrimitivePtr;

//=============================================================================================================================================//
//  VAO Pointer.																															   //
//=============================================================================================================================================//

class VertexArrayObjectPtr 
{ 
public:

	// Get the type of the VAO.
	GLenum getBufferType() { return m_bufferType; }
	
protected:

	friend class Renderer;

	// Constructor.
	VertexArrayObjectPtr(GLenum type) : m_bufferType(type) {}

	// Methods.
	inline virtual bool onDrawCall() = 0;

	// Data.
	GLenum m_bufferType = 0;						// Data type used in this VAO.	
	std::vector<PrimitivePtr*> m_primitives;		// Pointers to all of the primitives that have vertices in the VAO.	
	std::vector<PrimitivePtr*> m_primitivesToSync;	// Pointers to all of the primitives that have to be synced with the GPU.	
	unsigned m_VAOID = 0;							// Vertex Array Object ID.
	unsigned m_VBOID = 0;							// Vertex Buffer Objext ID.	
	unsigned m_IBOID = 0;							// Index Buffer Object ID.
	unsigned m_vertexCount = 0;						// The amount of vertices in this VAO.
	unsigned m_indexCount = 0;						// The amount of indices in this VAO.
	unsigned m_bufferIncrementSize = 0;				// How much the buffers increase/decrease in size.
	unsigned m_vertexBufferSize = 0;				// Amount of vertices contained in the buffer.
	unsigned m_indexBufferSize = 0;					// Amount of indices contained in the buffer.
	bool m_indexBufferSynced = true;				// Has the index buffer been synced?
	bool m_vertexBufferSynced = true;				// Has the vertexx buffer been synced?
	bool m_primitivesSynced = true;					// Checks if the primitives have been synced.
};

//=============================================================================================================================================//
//  VAO Class.																																   //
//=============================================================================================================================================//

template <class VertexType>
class VertexArrayObject : public VertexArrayObjectPtr
{	
public:

	// Constructor.
	VertexArrayObject(GLenum type);
	// Destructor.
	~VertexArrayObject();

	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// Draws the data in the VAO to the currently bound FBO.
	void render();
	// Binds the VAO.
	void bind() const;
	// Unbinds the VAO.
	void unbind() const;
	// Set the buffer size change steps.
	void setBufferIncrementSize(unsigned size);

	// --------- //
	//  D A T A  //
	// --------- //
	
	// Push the primitive for the VAO to keep track of.
	void pushPrimitive(PrimitivePtr* primitive, const std::vector<VertexType>& vertices, const std::vector<unsigned>& indices);
	// Remove the primitive from the VAO.
	void popPrimitive(PrimitivePtr* primitive);
	// Sets the primitive to be synced with the GPU.
	// Should be called after CPU data changed.
	void syncPrimitiveVertexData(PrimitivePtr* primitive);
	// Sync the IBO data of the primitive.
	void syncPrimitiveIndexData(PrimitivePtr* primitive);
	// Sync the vertex & index data.
	void syncPrimitiveData(PrimitivePtr* primitive);

	// This is in public so that primitives can manuipulate their
	// own vertices.  There should be a better way to handle this...
	// The vertices for the buffer stored on the CPU.
	std::vector<VertexType> m_vertexCPU;
	// The indices for the buffer stored on the CPU.
	std::vector<unsigned> m_indexCPU;

private:

	friend class Scene;
	friend class FrameBufferObject;
	friend class RendererStats;
	friend class Renderer;
	friend class BackgroundColorEditor;

	// ----------------------------------- //
	//  M E M O R Y   M A N A G E M E N T  //
	// ----------------------------------- //

	// Resizes the buffers on the GPU.
	bool queryBufferResize();
	// Updates the data on the GPU.
	void syncPrimitives();
	// Update the index buffer data.
	void syncIndexBuffer();
	// Update the index buffer data.
	void syncVertexBuffer();
	// This function deletes the data on the CPU side and keeps the GPU side data.
	// This is useful when no more changes are going to be made and RAM should be reduced.
	void wipeCPU();
	// Wipes all of the data GPU side but keeps the CPU data.
	void wipeGPU();
	// Wipes all of the data (CPU and GPU).
	void wipeAll();
	// Checks if buffer should resize, sync and render.
	virtual bool onDrawCall() override;
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
