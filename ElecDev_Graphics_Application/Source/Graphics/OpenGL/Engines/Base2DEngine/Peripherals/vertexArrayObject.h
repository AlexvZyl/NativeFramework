#pragma once

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include <vector>
#include <ErrorHandler/ErrorHandler.h>
#include <glad/glad.h>

//----------------------------------------------------------------------------------------------------------------------
//  Data Structures.
//----------------------------------------------------------------------------------------------------------------------

// Structr that contains the untextured vertex data.
struct VertexData
{
	float position[3] = { 0,0,0 };
	float color[4] = { 0,0,0,0 };
	float raw[7] = { 0,0,0,0,0,0,0 };
	
	// Vertex data without texture.
	VertexData(float pos0, float pos1, float pos2, float col0, float col1, float col2, float col3)
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
	}

	// Convert the data so that OpenGL can store it in the VBO.
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
struct TexturedVertexData
{
	float position[3] = { 0,0,0 };
	float color[4] = { 0,0,0,0 };
	float texturePosition[2] = { 0,0 };
	float textureID = 0.0f;
	float raw[10] = { 0,0,0,0,0,0,0,0,0,0 };

	// Vertex data without texture.
	TexturedVertexData(float pos0, float pos1, float pos2, float col0, float col1, float col2, float col3, float texPos0, float texPos1, float texID)
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
		// Assign texture ID position.
		texturePosition[0] = texPos0;
		texturePosition[1] = texPos1;
		// Assigne texture ID.
		textureID = texID;
	}

	// Convert the data so that OpenGL can store it in the VBO.
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
	GLenum m_bufferType;
	
public:

	// Pointer that shows where in the buffer data need to be written.
	GLsizei m_bufferPtr = 0;
	
	// Constructor.
	VertexArrayObject(GLenum type, bool textured=false);

	// Destructor.
	~VertexArrayObject();

	// Function that can dynamically write to the vertex buffer binded to this VAO.
	void writeData(std::vector<VertexData> vertices);
	// For textured.
	void writeData(std::vector<TexturedVertexData> vertices);

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
