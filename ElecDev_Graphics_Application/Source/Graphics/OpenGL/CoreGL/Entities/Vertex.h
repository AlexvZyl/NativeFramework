#pragma once

/*
The parent class that contains all of the information that a vertex type can contain.
BufferLayout is used to setup the VAO.
*/

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "GLAD/glad.h"
#include <vector>
#include "GLM/glm.hpp"

//=============================================================================================================================================//
//  Vertex Specification.																													   //
//=============================================================================================================================================//

class Vertex
{
public:
/*	union {
		struct {
			glm::vec3 position = { 0.f, 0.f, 0.f };			// Position of the vertex in 3D space.
			glm::vec4 color = { 0.f, 0.f, 0.f, 0.f };		// Color of the vertex.
			glm::vec2 textureCoords = { 0.f,0.f };			// Position
			float textureID = 0.f;
			float fade = 0.f;
		}data;
		float rawData[11];
	};
	glm::vec3* position;					// Position of the vertex in 3D space.
	glm::vec4* color;							// Color of the vertex.*/
	// Common vertex attributes.
	unsigned int entityID = 0;					// ID associated with the vertex.
	Vertex();
	virtual int getTotalSize() { return 0; };	// The entire data size.
	virtual int getDataSize()  { return 0; };	// Size of the texture data (excluding the entity ID).
	virtual int getIDOffset()  { return 0; };	// Size of offset to the entity ID.
	virtual int getIDSize()    { return 0; };	// Size of the entity ID variable.					

	virtual const void* dataGL() { return (const void*)NULL; }	// Formats the data so that OpenGL can use it.
	const void* idGL() { return (const void*)&entityID; }		// Returns the ID in a format that OpenGL can use.
};

//=============================================================================================================================================//
//  Basic vertex. 																															   //
//=============================================================================================================================================//

class VertexData : public Vertex
{
private: 
	static int totalSizeVD;						// All of the data.
	static int dataSizeVD;						// All of the color texture data.
	static int idOffsetVD;						// Offset to the entity ID.
	static int idSizeVD;						// Size of the entity ID.

public:
	union {
		struct {
			glm::vec3 position = { 0.f, 0.f, 0.f };			// Position of the vertex in 3D space.
			glm::vec4 color = { 0.f, 0.f, 0.f, 0.f };		// Color of the vertex.
		}data;
		float rawData[7];
	};
	glm::vec3* position = &data.position;					// Position of the vertex in 3D space.
	glm::vec4* color = &data.color;							// Color of the vertex.
	// Constructor.
	VertexData();
	VertexData(glm::vec3& pos, glm::vec4& clr, unsigned int eID);
	VertexData(float pos0, float pos1, float pos2,
			   float col0, float col1, float col2, float col3,
			   unsigned int eID);
	
	virtual const void* dataGL() override;	// Return the raw data for OpenGL to use.
	virtual int getTotalSize() override;	// The entire data size.
	virtual int getDataSize() override;		// Size of the texture data (excluding the entity ID).
	virtual int getIDOffset() override;		// Size of offset to the entity ID.
	virtual int getIDSize() override;		// Size of the entity ID variable.			
};

//=============================================================================================================================================//
//  Textured basic vertex. 																													   //
//=============================================================================================================================================//

class VertexDataTextured : public Vertex
{
private:
	static int totalSizeVDT;	// All of the data.
	static int dataSizeVDT;		// All of the color texture data.
	static int idOffsetVDT;		// Offset to the entity ID.
	static int idSizeVDT;		// Size of the entity ID.

public:
	union {
		struct {
			glm::vec3 position = { 0.f, 0.f, 0.f };			// Position of the vertex in 3D space.
			glm::vec4 color = { 0.f, 0.f, 0.f, 0.f };		// Color of the vertex.
			glm::vec2 textureCoords = { 0.f,0.f };			// Position
			float textureID = 0.f;
		}data;
		float rawData[10];
	};
	glm::vec3* position = &data.position;					// Position of the vertex in 3D space.
	glm::vec4* color = &data.color;							// Color of the vertex.

	glm::vec2* textureCoords = &data.textureCoords;			// Position
	float* textureID = &data.textureID;
	// Constructors.
	VertexDataTextured();
	VertexDataTextured(glm::vec3& pos, glm::vec4& clr, glm::vec2& texCoords, float texID, unsigned int eID);
	VertexDataTextured(float pos0, float pos1, float pos2,
		float col0, float col1, float col2, float col3,
		float texPos0, float texPos1, float texID,
		unsigned int eID);
	
	virtual const void* dataGL() override;	// Return the raw data for OpenGL to use.
	virtual int getTotalSize() override;	// The entire data size.
	virtual int getDataSize() override;		// Size of the texture data (excluding the entity ID).
	virtual int getIDOffset() override;		// Size of offset to the entity ID.
	virtual int getIDSize() override;		// Size of the entity ID variable.			
};

//=============================================================================================================================================//
//  Circle Vertex Data																														   //
//=============================================================================================================================================//

// The circle vertex data requires a local coordinate system.
class VertexDataCircle : public Vertex
{
private:
	static int totalSizeVDC;	// All of the data.
	static int dataSizeVDC;		// All of the color texture data.
	static int idOffsetVDC;		// Offset to the entity ID.
	static int idSizeVDC;		// Size of the entity ID.

public:

	union {
		struct {
			glm::vec3 position = { 0.f, 0.f, 0.f };			// Position of the vertex in 3D space.
			glm::vec4 color = { 0.f, 0.f, 0.f, 0.f };		// Color of the vertex.
			glm::vec2 localCoords = { 0.f,0.f };			// Position
			float thickness = 0.f;
			float fade = 0.f;
		}data;
		float rawData[11];
	};
	glm::vec3* position = &data.position;					// Position of the vertex in 3D space.
	glm::vec4* color = &data.color;							// Color of the vertex.
	glm::vec2* localCoords = &data.localCoords;			// Position
	float* thickness = &data.thickness;
	float* fade = &data.fade;
	// Constructors.
	//VertexDataCircle() = default;
	VertexDataCircle(glm::vec3& Position, glm::vec2& local, glm::vec4& Color, float Thickness, float Fade, unsigned int eID);

	virtual const void* dataGL() override;	// Return the raw data for OpenGL to use.
	virtual int getTotalSize() override;	// The entire data size.
	virtual int getDataSize() override;		// Size of the texture data (excluding the entity ID).
	virtual int getIDOffset() override;		// Size of offset to the entity ID.
	virtual int getIDSize() override;		// Size of the entity ID variable.			
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
