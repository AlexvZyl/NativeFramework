#pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <glad/glad.h>
#include <vector>
#include <glm.hpp>
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

	// Constructor.
	VertexArrayObjectPtr(GLenum type) : m_bufferType(type) {}
	// Get the type of the VAO.
	GLenum getBufferType() { return m_bufferType; }
	
private:

	// Friends.
	template <class VertexType>
	friend class VertexArrayObject;

	// Data.
	GLenum m_bufferType = 0;				       // Data type used in this VAO.	
	std::vector<PrimitivePtr*> m_primitives;       // Pointers to all of the primitives that have vertices in the VAO.	
	std::vector<PrimitivePtr*> m_primitivesToSync; // Pointers to all of the primitives that have to be synced with the GPU.	
	unsigned int m_VAOID = 0;					   // Vertex Array Object ID.
	unsigned int m_VBOID = 0;					   // Vertex Buffer Objext ID.	
	unsigned int m_IBOID = 0;					   // Index Buffer Object ID.
	unsigned int m_vertexCount = 0;				   // The amount of vertices in this VAO.
	unsigned int m_indexCount = 0;				   // The amount of indices in this VAO.
	bool m_synced = true;						   // Checks if there is data CPU side that has not been updated GPU side.
	bool m_sized = true;						   // Checks if the buffers have to be resized 
};

//=============================================================================================================================================//
//  VAO Class.																																   //
//=============================================================================================================================================//

template <class VertexType>
class VertexArrayObject : public VertexArrayObjectPtr
{	
public:

	// ------------------------------------------------- //
	//  C O N S T R U C T O R   &   D E S T R U C T O R  //
	// ------------------------------------------------- //

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
	// Sets the primitive to be updated.
	// Updates are done per frame.
	void sync(PrimitivePtr* primitive);
	// Sets the buffers to be resized based on the data on the CPU.
	// Updates are done per frame.
	void resize();

	// --------- //
	//  D A T A  //
	// --------- //

	// Append the vertex data to the buffer.
	// It returns the position of the vertex data in the vector.
	void appendVertexData(std::vector<std::unique_ptr<VertexType>>& vertices, std::vector<unsigned>& indices,
						  unsigned* vertexPos = nullptr, unsigned* indexPos = nullptr);
	// Removes vertex data from the VBO.
	void deleteVertexData(unsigned vertexPos, unsigned vertexCount,
						  unsigned indexPos, unsigned indexCount);
	// Adds a pointer of the primitive to the VAO.
	void pushPrimitive(PrimitivePtr* primitive);
	// Remove the primitive from the VAO.
	void popPrimitive(int primitiveIndex, int vertexCount, int indexCount);

	// The vertices for the buffer stored on the CPU.
	std::vector<std::unique_ptr<VertexType>> m_vertexCPU;
	// The indices for the buffer stored on the CPU.
	std::vector<unsigned> m_indexCPU;

private:

	// ----------------------------------- //
	//  M E M O R Y   M A N A G E M E N T  //
	// ----------------------------------- //

	// Resizes the buffers on the GPU.
	void resizeBuffer();
	// Updates the data on the GPU.
	void syncBuffer();
	// This function deletes the data on the CPU side and keeps the GPU side data.
	// This is useful when no more changes are going to be made and RAM should be reduced.
	void wipeCPU();
	// Wipes all of the data GPU side but keeps the CPU data.
	void wipeGPU();
	// Wipes all of the data (CPU and GPU).
	void wipe();
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
