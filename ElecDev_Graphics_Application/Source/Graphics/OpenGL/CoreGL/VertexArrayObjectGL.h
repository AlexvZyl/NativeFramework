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
class Entity;
class Vertex;

//=============================================================================================================================================//
//  Class.																																	   //
//=============================================================================================================================================//

template <class VertexType>
class VertexArrayObject
{
private:
	
	unsigned int m_VAOID;		// VAO ID.
	unsigned int m_VBOID;		// VBO ID.	
	GLsizei m_bufferIndex = 0;	// Pointer that shows where in the buffer data need to be written.
	bool m_isUpdated = false;		// Checks if there is data CPU side that has not been updated GPU side.

public:

	GLenum m_bufferType = 0;		// Data type used in this VAO.	

	// --------- //
	//  D A T A  //
	// --------- //

	// Vertices stores CPU side.
	std::vector<VertexType> m_vertexCPU;
	// Entities stored CPU side.
	std::vector<Entity<VertexType>*> m_entityCPU;

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
	void appendDataCPU(std::vector<VertexType>* vertices);
	// Assign data to the CPU side memory for untextured vertices.
	// If the VAO is empty it creates a new one with the vertices specified.
	void assignDataCPU(std::vector<VertexType>* vertices, unsigned int index);
	// Assign data to the GPU side memory for untextured vertices.
	// since this funtion is going to be called in performance critical scenarios.
	// If you use this function it will update faster, but you also lose the ability
	// to read the CPU side data.
	// Error handling is not added to this function to make it as fast as possible, so
	// be very careful to not lose track of sizes of data, if it has been created etc.
	// This does not check if the array is empty and create a new one if it is empty.
	void assignDataGPU(std::vector<VertexType>* vertices, unsigned int index);

	// ----------------- //
	//  E N T I T I E S  //
	// ----------------- //

	// Append data on the CPU side memory for textured vertices.
	void appendDataCPU(Entity<VertexType>* entity);
	// Assign data to the GPU side memory for textured vertices.
	// since this funtion is going to be called in performance critical scenarios.
	// If you use this function it will update faster, but you also lose the ability
	// to read the CPU side data.
	// Error handling is not added to this function to make it as fast as possible, so
	// be very careful to not lose track of sizes of data, if it has been created etc.
	// This does not check if the array is empty and create a new one if it is empty.
	void assignDataGPU(Entity<VertexType>* entity);
	// Delete the polygon from the VAO.
	void deleteDataCPU(Entity<VertexType>* entity);
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//