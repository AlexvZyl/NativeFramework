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
	bool m_inSync = true;			// Checks if there is data CPU side that has not been updated GPU side.

public:

	GLenum m_bufferType = 0;		// Data type used in this VAO.	

	// --------- //
	//  D A T A  //
	// --------- //

	// Vertices stores CPU side.
	std::vector<VertexType> m_vertexCPU;
	// The indeces bot the buffer.
	std::vector<unsigned> m_indexCPU;
	// Entities stored CPU side.
	std::vector<Primitive<VertexType>*> m_entityCPU;

	// ------------------------------------------------- //
	//  C O N S T R U C T O R   &   D E S T R U C T O R  //
	// ------------------------------------------------- //

	// Constructor.
	VertexArrayObject();
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
	// Sets m_isUpdated to false for when something changes externally.
	void outOfSync();

	// ----------------------------------- //
	//  M E M O R Y   M A N A G E M E N T  //
	// ----------------------------------- //

	// This function deletes the data on the CPU side for when only the GPU is being updated and
	// the CPU side data is no longer required.
	// Be careful when calling this function!
	void wipeCPU();
	// Sends the CPU data to the GPU.
	void updateGPU();

	// ----------------- //
	//  V E R T I C E S  //
	// ----------------- //

	// Append data on the CPU side memory for untextured vertices.
	void appendDataCPU(std::vector<VertexType>& vertices, std::vector<unsigned> indices);

	// ----------------- //
	//  E N T I T I E S  //
	// ----------------- //

	// Append data on the CPU side memory for textured vertices.
	void appendDataCPU(Primitive<VertexType>* entity);
	// Delete the polygon from the VAO.
	void deleteDataCPU(Primitive<VertexType>* entity);
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//