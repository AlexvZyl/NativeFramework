#pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <vector>
#include "ErrorHandlerGL.h"
#include <glad/glad.h>
#include <glm.hpp>

//=============================================================================================================================================//
//  Forward declerations.																													   //
//=============================================================================================================================================//

class Entity;

//=============================================================================================================================================//
//  Data structures.																														   //
//=============================================================================================================================================//

struct Colour
{
	float R, G, B, A;
	// Default constructor.
	Colour() { R = 0; G = 0; B = 0; A = 1; }
	//Creates a colour with the specified values.
	Colour(float r, float g, float b, float a)
	{ R = r; G = g; B = b; A = a; }
};

// Structure that contains the untextured vertex data.
struct VertexData
{
	glm::vec3 position = { 0,0,0 };
	Colour color = Colour( 0,0,0,0 );
	float raw[7] = { 0,0,0,0,0,0,0 };
	unsigned int entityID[1] = { 0 };

	// Default.
	VertexData() {};

	// Adds vertex data from floats.
	VertexData(float pos0, float pos1, float pos2, 
			   float col0, float col1, float col2, float col3, 
			   unsigned int eID)
	{
		// Assign position.
		position.x = pos0; position.y = pos1; position.z = pos2;
		// Assign color.
		color.R = col0;
		color.G = col1;
		color.B = col2;
		color.A = col3;
		// Assign ID.
		entityID[0] = eID;
	}

	//Adds vertex data from position and colour structures
	VertexData(glm::vec3 pos, Colour col, unsigned int eID)
	{
		// Assign position.
		position = pos;
		// Assign color.
		color = col;
		// Assign ID.
		entityID[0] = eID;
	}

	// Return a pointer to the raw data so that OpenGL can write it to the GPU.
	const void* rawData()
	{
		raw[0] = position.x;
		raw[1] = position.y;
		raw[2] = position.z;
		raw[3] = color.R;
		raw[4] = color.G;
		raw[5] = color.B;
		raw[6] = color.A;
		return (const void*)raw;
	}
};

// Struct that contains the textured vertex data.
struct VertexDataTextured
{
	float position[3] = { 0,0,0 };
	float color[4] = { 0,0,0,0 };
	float texturePosition[2] = { 0,0 };
	float textureID = 0.0f;
	float raw[10] = { 0,0,0,0,0,0,0,0,0,0 };
	unsigned int entityID[1] = { 0 };

	// Constructor.
	VertexDataTextured(float pos0, float pos1, float pos2,
		float col0, float col1, float col2, float col3,
		float texPos0, float texPos1, float texID,
		unsigned int eID)
	{
		// Assign position.
		position[0] = pos0; position[1] = pos1; position[2] = pos2;
		// Assign color.
		color[0] = col0; color[1] = col1; color[2] = col2; color[3] = col3;
		// Assign entity ID.
		entityID[0] = eID;
		// Assign texture ID position.
		texturePosition[0] = texPos0; texturePosition[1] = texPos1;
		// Assigne texture ID.
		textureID = (unsigned int)texID;
	}

	// Return a pointer to the raw data so that OpenGL can write it to the GPU.
	const void* rawData()
	{
		raw[0] = position[0]; raw[1] = position[1]; raw[2] = position[2];
		raw[3] = color[0]; raw[4] = color[1]; raw[5] = color[2]; raw[6] = color[3];
		raw[7] = texturePosition[0]; raw[8] = texturePosition[1];
		raw[9] = textureID;
		return (const void*)raw;
	}
};

//=============================================================================================================================================//
//  Class.																																	   //
//=============================================================================================================================================//

class VertexArrayObject
{
private:

	unsigned int m_vAID;	 // VAO ID.
	unsigned int m_vBID;	 // VBO ID.
	GLenum m_bufferType;	 // Data type used in this VAO.		
	GLsizei m_bufferIndex = 0; // Pointer that shows where in the buffer data need to be written.

public:

	// --------- //
	//  D A T A  //
	// --------- //

	// Data stored CPU side.  This allows the buffers to be resized dynamically as more or less memory is required.
	// Data should not be stored and read from OpenGL (GPU side).
	std::vector<VertexData> m_vertexDataCPU;
	std::vector<VertexDataTextured> m_vertexDataTexturedCPU;
	std::vector<Entity*> m_entityCPU;

	// ------------------------------------------------- //
	//  C O N S T R U C T O R   &   D E S T R U C T O R  //
	// ------------------------------------------------- //

	// Constructor.
	VertexArrayObject(GLenum type, bool textured = false);
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
	void appendDataCPU(std::vector<VertexData> vertices);
	// Assign data to the CPU side memory for untextured vertices.
	// If the VAO is empty it creates a new one with the vertices specified.
	void assignDataCPU(std::vector<VertexData> vertices, unsigned int index);
	// Assign data to the GPU side memory for untextured vertices.
	// since this funtion is going to be called in performance critical scenarios.
	// If you use this function it will update faster, but you also lose the ability
	// to read the CPU side data.
	// Error handling is not added to this function to make it as fast as possible, so
	// be very careful to not lose track of sizes of data, if it has been created etc.
	// This does not check if the array is empty and create a new one if it is empty.
	void assignDataGPU(std::vector<VertexData> vertices, unsigned int index);

	// ----------------------------------- //
	//  T E X T U R E D   V E R T I C E S  //
	// ----------------------------------- //
	
	// Append data on the CPU side memory for textured vertices.
	void appendDataCPU(std::vector<VertexDataTextured> vertices);
	// Assign data to the CPU side memory for untextured vertices.
	// If the VAO is empty it creates a new one with the vertices specified.
	void assignDataCPU(std::vector<VertexDataTextured> vertices, unsigned int index);
	// Assign data to the GPU side memory for textured vertices.
	// since this funtion is going to be called in performance critical scenarios.
	// If you use this function it will update faster, but you also lose the ability
	// to read the CPU side data.
	// Error handling is not added to this function to make it as fast as possible, so
	// be very careful to not lose track of sizes of data, if it has been created etc.
	// This does not check if the array is empty and create a new one if it is empty.
	void assignDataGPU(std::vector<VertexDataTextured> vertices, unsigned int index);

	// ----------------- //
	//  P O L Y G O N S  //
	// ----------------- //

	// Append data on the CPU side memory for textured vertices.
	void appendDataCPU(Entity* entity);
	// Assign data to the GPU side memory for textured vertices.
	// since this funtion is going to be called in performance critical scenarios.
	// If you use this function it will update faster, but you also lose the ability
	// to read the CPU side data.
	// Error handling is not added to this function to make it as fast as possible, so
	// be very careful to not lose track of sizes of data, if it has been created etc.
	// This does not check if the array is empty and create a new one if it is empty.
	void assignDataGPU(Entity* entity);
	// Delete the polygon from the VAO.
	void deleteDataCPU(Entity* entity);
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//