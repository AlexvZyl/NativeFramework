#pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <glad/glad.h>
#include <vector>
#include <glm.hpp>
#include <memory>

//=============================================================================================================================================//
//  Forward decleration.																													   //
//=============================================================================================================================================//

template<typename VertexType>
class Primitive;
class Vertex;

class Entity;

//=============================================================================================================================================//
//  Class.																																	   //
//=============================================================================================================================================//

template <class VertexType>
class VertexArrayObject
{
private:

	unsigned int m_VAOID;			// Vertex Array Object.
	unsigned int m_VBOID;			// Vertex Buffer Objext.	
	unsigned int m_IBOID;			// Index Buffer Object.
	unsigned int m_vertexCount = 0;	// Pointer that shows where in the buffer data need to be written.
	unsigned int m_indexCount = 0;	// Counting the amount of indices.
	bool m_synced = true;			// Checks if there is data CPU side that has not been updated GPU side.
	bool m_sized = true;			// Checks if the data has to be resized 

public:

	GLenum m_bufferType = 0;		// Data type used in this VAO.	

	// --------- //
	//  D A T A  //
	// --------- //

	// Vertices stores CPU side.
	std::vector<VertexType> m_vertexCPU;
	// The indeces bot the buffer.
	std::vector<unsigned> m_indexCPU;

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

	// Draws the data in the VAO.
	void render();
	// Binds the VAO.
	void bind() const;
	// Unbinds the VAO.
	void unbind() const;
	// Sets the buffers to be updated.
	void sync();
	// Sets the buffers to be resized.
	void resize();

	// ----------------------------------- //
	//  M E M O R Y   M A N A G E M E N T  //
	// ----------------------------------- //

	// Resizes the buffers on the GPU.
	void resizeGPU();
	// Updates the data on the GPU.
	void syncGPU();
	// This function deletes the data on the CPU side for when only the GPU is being updated and
	// the CPU side data is no longer required.
	// Be careful when calling this function!
	void wipeCPU();
	// Wipes all of the data (CPU and GPU).
	void wipe();

	// --------- //
	//  D A T A  //
	// --------- //

	// Append the vertex data to the buffer.
	// It returns the position of the vertex data in the vector.
	void appendVertexData(std::vector<VertexType>& vertices, std::vector<unsigned>& indices,
						  unsigned* vertexPos = nullptr, unsigned* indexPos = nullptr);
	// Removes vertex data from the VBO.
	void deleteVertexData(unsigned vertexPos, unsigned vertexCount,
						  unsigned indexPos, unsigned indexCount);
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
