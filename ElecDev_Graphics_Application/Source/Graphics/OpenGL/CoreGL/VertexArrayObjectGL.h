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

//Vector definitions
//Note: These could be moved, they are fairly general

//Basic 2D vector
struct float2
{
	float x;
	float y;

	// Constructors.

	//creates ininitialised float2
	float2() {}

	float2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
};

//basic 3D vector
struct float3
{
	float x;
	float y;
	float z;

	// Constructors.

	//creates ininitialised float3
	float3() {	}

	//Creates a float3 with the specified values.
	float3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	//Creates a float3 with x and y copied from a float2 plus the specified z value.
	float3(float2 val, float z)
	{
		this->x = val.x;
		this->y = val.y;
		this->z = z;
	}

};

struct Colour
{
	float r;
	float g;
	float b;
	float a;

	// Constructors.

	//creates ininitialised colour
	Colour() {	}

	//Creates a colour with the specified values.
	Colour(float r, float g, float b, float a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
	
	//Could add another constructors that sets a specified colour defined in a string
	//i.e. Colour(char* colour , float a)
};





// Structure that contains the untextured vertex data.
struct VertexData
{
	float position[3] = { 0,0,0 };
	float color[4] = { 0,0,0,0 };
	float raw[7] = { 0,0,0,0,0,0,0 };
	unsigned int entityID[1] = { 0 };
	
	// Constructors.

	//create an unititialised VertexData structure
	VertexData() {};

	//Adds vertex data from floats
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

	//Adds vertex data from position and colour structures
	VertexData(float3 pos, Colour col, unsigned int eID)
	{
		// Assign position.
		position[0] = pos.x;
		position[1] = pos.y;
		position[2] = pos.z;
		// Assign color.
		color[0] = col.r;
		color[1] = col.g;
		color[2] = col.b;
		color[3] = col.a;
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

	// Append data on the CPU side memory for untextured vertices.
	void writeData(std::vector<VertexData> vertices);
	// Append data on the CPU side memory for textured vertices.
	void writeData(std::vector<VertexDataTextured> vertices);
	// Assign data to the CPU side memory for untextured vertices.
	void assignData(std::vector<VertexData> vertices);
	// Assign data to the CPU side memory for untextured vertices.
	void assignData(std::vector<VertexDataTextured> vertices);
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
