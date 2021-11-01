#pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <vector>
#include "ErrorHandlerGL.h"
#include <glad/glad.h>

//=============================================================================================================================================//
//  Data structures.																														   //
//=============================================================================================================================================//

// Structure that contains the untextured vertex data.
struct VertexData
{
	float position[3] = { 0,0,0 };
	float color[4] = { 0,0,0,0 };
	float raw[7] = { 0,0,0,0,0,0,0 };
	unsigned int entityID[1] = { 0 };
	
	// Constructor.
	VertexData(float pos0, float pos1, float pos2, 
			   float col0, float col1, float col2, float col3, 
			   unsigned int eID)
	{
		// Assign position.
		position[0] = pos0;
		position[1] = pos1;
		position[2] = pos2;
		// Assign color.
		color[0] = col0;
		color[1] = col1;
		color[2] = col2;
		color[3] = col3;
		// Assign ID.
		entityID[0] = eID;
	}

	// Return a pointer to the raw data so that OpenGL can write it to the GPU.
	const void* rawData()
	{
		raw[0] = position[0];
		raw[1] = position[1];
		raw[2] = position[2];
		raw[3] = color[0];
		raw[4] = color[1];
		raw[5] = color[2];
		raw[6] = color[3];
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
		position[0] = pos0;
		position[1] = pos1;
		position[2] = pos2;
		// Assign color.
		color[0] = col0;
		color[1] = col1;
		color[2] = col2;
		color[3] = col3;
		// Assign entity ID.
		entityID[0] = eID;
		// Assign texture ID position.
		texturePosition[0] = texPos0;
		texturePosition[1] = texPos1;
		// Assigne texture ID.
		textureID = (unsigned int)texID;
	}

	// Return a pointer to the raw data so that OpenGL can write it to the GPU.
	const void* rawData()
	{
		raw[0] = position[0];
		raw[1] = position[1];
		raw[2] = position[2];
		raw[3] = color[0];
		raw[4] = color[1];
		raw[5] = color[2];
		raw[6] = color[3];
		raw[7] = texturePosition[0];
		raw[8] = texturePosition[1];
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
	
	unsigned int m_vAID;	// VAO ID.
	unsigned int m_vBID;	// VBO ID.
	GLenum m_bufferType;	// Data type used in this VAO.		
	
public:

	// --------- //
	//  D A T A  //
	// --------- //

	// Data stored CPU side.  This allows the buffers to be resized dynamically as more or less memory is required.
	// Data should not be stored and read from OpenGL (GPU side).
	std::vector<VertexData> m_vertexDataCPU;
	std::vector<VertexDataTextured> m_VertexDataTexturedCPU;
	// Pointer that shows where in the buffer data need to be written.
	GLsizei m_bufferPtr = 0;
	
	// ------------------------------------------------- //
	//  C O N S T R U C T O R   &   D E S T R U C T O R  //
	// ------------------------------------------------- //

	// Constructor.
	VertexArrayObject(GLenum type, bool textured=false);		
	// Destructor.
	~VertexArrayObject();										

	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// Store the data on the CPU side memory for untextured vertices.
	void writeData(std::vector<VertexData> vertices);			
	// Store the data on the CPU side memory for textured vertices.
	void writeData(std::vector<VertexDataTextured> vertices);   
	// Sends the data to the GPU.
	void updateGPU();											
	// Draws the data in the VAO.
	void render();												
	// Binds the VAO.
	void bind() const;											
	// Unbinds the VAO.
	void unbind() const;										
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
