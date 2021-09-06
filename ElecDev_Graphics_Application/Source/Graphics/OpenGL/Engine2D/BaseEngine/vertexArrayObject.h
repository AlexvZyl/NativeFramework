#pragma once

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include <ErrorHandler/errorHandler.h>
//#include <glad/glad.h>
#include <GLEW/include/GL/glew.h>

//----------------------------------------------------------------------------------------------------------------------
//  Data Structures.
//----------------------------------------------------------------------------------------------------------------------

// Types of Vertex Arrays that can be created.
enum class BufferType 
{
	LINES=GL_LINES, TRIANGLE_CLEAR=GL_LINES, TRIANGLE_FILLED=GL_TRIANGLES, QUAD_CLEAR=GL_LINES, QUAD_FILLED=GL_QUADS, CIRCLE_CLEAR= GL_LINES, CIRCLE_FILLED = GL_TRIANGLES, TEXT=GL_QUADS
};

// Struct that contains the layout of the line vertex data.
struct VertexData
{
	float position[3] = {0,0,0};
	float color[4] = {0,0,0,0};
	float raw[7] = {0,0,0,0,0,0,0};
	
	// Default constructor.
	VertexData() {};
	// Constructor for the vertex data structure.
	VertexData(float v0, float v1, float v2, float v3, float v4, float v5, float v6)
	{
		// Assign position.
		position[0] = v0;
		position[1] = v1;
		position[2] = v2;
		// Assign color.
		color[0] = v3;
		color[1] = v4;
		color[2] = v5;
		color[3] = v6;
	}

	// Convert the data so that OpenGL can store it in the VBO.
	const void* rawData() 
	{
		float raw[7] = { position[0], position[1], position[2], color[0], color[1], color[2], color[3] };
		return (const void*) raw;
	}

};

//----------------------------------------------------------------------------------------------------------------------
//  Vertex Array Class.
//----------------------------------------------------------------------------------------------------------------------

class VertexArrayObject
{
private:
	
	// VAO ID.
	unsigned int m_vAID;
	// VBO ID.
	unsigned int m_vBID;
	// Data type used in this VAO.
	BufferType m_BufferType;
	
public:

	// Total vertices available.
	unsigned int m_maxVertices;
	// Pointer that shows where in the buffer data need to be written.
	GLsizei m_bufferPtr = 0;
	
	// Constructor.
	VertexArrayObject(BufferType type, unsigned int maxVertices);

	// Destructor.
	~VertexArrayObject();

	// Function that can dynamically write to the vertex buffer binded to this VAO.
	void writeData(VertexData v);
	void writeData(VertexData v1, VertexData v2);
	void writeData(VertexData v1, VertexData v2, VertexData v3);
	void writeData(VertexData v1, VertexData v2, VertexData v3, VertexData v4);

	// Function that draws the data in the VAO based on the model type selected in the constructor.
	void render();

	// Binds the VAO.
	void bind() const;
	// Unbinds the VAO.
	void unbind() const;

};

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------
