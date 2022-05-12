#pragma once

/*
The parent class that contains all of the information that a vertex type can contain.
BufferLayout is used to setup the VAO.
*/

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <vector>
#include "GLM/glm.hpp"

//=============================================================================================================================================//
//  Vertex Specification.																													   //
//=============================================================================================================================================//

class Vertex
{
public:

	// ID associated with the vertex.
	unsigned int entityID = 0;									
	// Constructor.
	Vertex() = default;

	// --------- //
	//  D A T A  //
	// --------- //
	
	// Size of the texture data (excluding the entity ID).
	inline virtual int getDataSize() = 0;					
	// Size of offset to the entity ID.
	inline virtual int getIDOffset() = 0;
	// Size of the entity ID variable.					
	inline  virtual int getIDSize() = 0;
	// Formats the data so that OpenGL can use it.
	inline virtual const void* getData() = 0;
	// Returns the ID in a format that OpenGL can use.
	const void* getID() { return (const void*)&entityID; }			
};

//=============================================================================================================================================//
//  Basic vertex. 																															   //
//=============================================================================================================================================//

class VertexData : public Vertex
{
private: 
	
	// Sizes.
	static int totalSizeVD;		// All of the data.
	static int dataSizeVD;		// All of the color texture data.
	static int idOffsetVD;		// Offset to the entity ID.
	static int idSizeVD;		// Size of the entity ID.

public:

	// The vertex data.
	union {
		struct {
			glm::vec3 position = { 0.f, 0.f, 0.f };			// Position of the vertex in 3D space.
			glm::vec4 color = { 0.f, 0.f, 0.f, 0.f };		// Color of the vertex.
			float outline = 0.f;
		}data;
		float rawData[8];
	};

	// Pointers to the data.
	glm::vec3* position = &data.position;					// Position of the vertex in 3D space.
	glm::vec4* color = &data.color;							// Color of the vertex.
	float* outline = &data.outline;							

	// Constructor.
	VertexData(const glm::vec3& pos, const glm::vec4& clr, unsigned int eID);
	
	// Return the raw data for OpenGL to use.
	virtual const void* getData() override;	
	// The entire data size.
	static int getTotalSize();	
	// Size of the texture data (excluding the entity ID).
	virtual int getDataSize() override;		
	// Size of offset to the entity ID.
	virtual int getIDOffset() override;		
	// Size of the entity ID variable.			
	virtual int getIDSize() override;		
	// Sets the attributes according to the vertex data.
	static void initVertexAttributes(unsigned vao);
};

//=============================================================================================================================================//
//  Textured basic vertex. 																													   //
//=============================================================================================================================================//

class VertexDataTextured : public Vertex
{
private:

	// Sizes.
	static int totalSizeVDT;	// All of the data.
	static int dataSizeVDT;		// All of the color texture data.
	static int idOffsetVDT;		// Offset to the entity ID.
	static int idSizeVDT;		// Size of the entity ID.

public:

	// The vertex data.
	union {
		struct {
			glm::vec3 position = { 0.f, 0.f, 0.f };			// Position of the vertex in 3D space.
			glm::vec4 color = { 0.f, 0.f, 0.f, 0.f };		// Color of the vertex.
			glm::vec2 textureCoords = { 0.f,0.f };			// Position
			float outline = 0.f;
			float textureID = 0.f;
		}data;
		float rawData[11];
	};

	// Pointers to the data.
	glm::vec3* position = &data.position;					// Position of the vertex in 3D space.
	glm::vec4* color = &data.color;							// Color of the vertex.
	glm::vec2* textureCoords = &data.textureCoords;			// Position
	float* textureID = &data.textureID;
	float* outline = &data.outline;

	// Constructor.
	VertexDataTextured(const glm::vec3& pos, const glm::vec4& color, const glm::vec2& texCoords, float texID, unsigned int eID);
	
	// Return the raw data for OpenGL to use.
	virtual const void* getData() override;	
	// The entire data size.
	static int getTotalSize();	
	// Size of the texture data (excluding the entity ID).
	virtual int getDataSize() override;		
	// Size of offset to the entity ID.
	virtual int getIDOffset() override;		
	// Size of the entity ID variable.			
	virtual int getIDSize() override;	
	// Sets the attributes according to the vertex data.
	static void initVertexAttributes(unsigned vao);
};

//=============================================================================================================================================//
//  Circle Vertex Data																														   //
//=============================================================================================================================================//

// The circle vertex data requires a local coordinate system.
class VertexDataCircle : public Vertex
{
private:

	// Data sizes.
	static int totalSizeVDC;	// All of the data.
	static int dataSizeVDC;		// All of the color texture data.
	static int idOffsetVDC;		// Offset to the entity ID.
	static int idSizeVDC;		// Size of the entity ID.

public:

	// The vertex data.
	union {
		struct {
			glm::vec3 position = { 0.f, 0.f, 0.f };			// Position of the vertex in 3D space.
			glm::vec4 color = { 0.f, 0.f, 0.f, 0.f };		// Color of the vertex.
			float radius = 0.f;
			glm::vec2 localCoords = { 0.f,0.f };			// Position
			float outline = 0.f;
			float thickness = 0.f;
			float fade = 0.f;
		}data;
		float rawData[13];
	};

	// Pointers to the data.
	glm::vec3* position = &data.position;			// Position of the vertex in 3D space.
	glm::vec4* color = &data.color;					// Color of the vertex.
	float* radius = &data.radius;
	glm::vec2* localCoords = &data.localCoords;		// Position
	float* thickness = &data.thickness;
	float* fade = &data.fade;
	float* outline = &data.outline;

	// Constructors.
	VertexDataCircle(const glm::vec3& Position, const glm::vec2& local, float radius, const glm::vec4& Color, float Thickness, float Fade, unsigned int eID);

	// Return the raw data for OpenGL to use.
	virtual const void* getData() override;	
	// The entire data size.
	static int getTotalSize();	
	// Size of the texture data (excluding the entity ID).
	virtual int getDataSize() override;		
	// Size of offset to the entity ID.
	virtual int getIDOffset() override;		
	// Size of the entity ID variable.			
	virtual int getIDSize() override;	
	// Sets the attributes according to the vertex data.
	static void initVertexAttributes(unsigned vao);
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
