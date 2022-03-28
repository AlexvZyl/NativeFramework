//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "OpenGL/Primitives/Vertex.h"
#include "External/GLAD/Includes/glad/glad.h"
#include "OpenGL/ErrorHandlerGL.h"

//=============================================================================================================================================//
//  Static values.																															   //
//=============================================================================================================================================//

// VertexData.
int VertexData::idSizeVD					= sizeof(VertexData::entityID);
int VertexData::dataSizeVD					= sizeof(*VertexData::position) + sizeof(*VertexData::color) + sizeof(*VertexData::outline);
int VertexData::idOffsetVD					= VertexData::dataSizeVD;
int VertexData::totalSizeVD					= VertexData::dataSizeVD + VertexData::idSizeVD;									

// VertexDataTextured.
int VertexDataTextured::idSizeVDT			= sizeof(VertexDataTextured::entityID);
int VertexDataTextured::dataSizeVDT			= sizeof(*VertexDataTextured::position) + sizeof(*VertexDataTextured::color)
											+ sizeof(*VertexDataTextured::textureCoords) + sizeof(*VertexDataTextured::textureID) 
											+ sizeof(*VertexDataTextured::outline);
int VertexDataTextured::idOffsetVDT			= VertexDataTextured::dataSizeVDT;
int VertexDataTextured::totalSizeVDT		= VertexDataTextured::dataSizeVDT + VertexDataTextured::idSizeVDT;

// Vertex Data Circle.
int VertexDataCircle::idSizeVDC				= sizeof(VertexDataCircle::entityID);
int VertexDataCircle::dataSizeVDC			= sizeof(*VertexDataCircle::position) + sizeof(*VertexDataCircle::color) + sizeof(*VertexDataCircle::localCoords)
											+ sizeof(*VertexDataCircle::fade) + sizeof(*VertexDataCircle::fade) + sizeof(*VertexDataCircle::outline);
int VertexDataCircle::idOffsetVDC			= VertexDataCircle::dataSizeVDC;
int VertexDataCircle::totalSizeVDC			= VertexDataCircle::dataSizeVDC + VertexDataCircle::idSizeVDC;

//=============================================================================================================================================//
//  Vertex Data.																															   //
//=============================================================================================================================================//

VertexData::VertexData(const glm::vec3& pos, const glm::vec4& colorIn, unsigned int eID)
{
	*position = pos; *color = colorIn; entityID = eID; *outline = 0.f;
}

const void* VertexData::getData()
{
	return (const void*)rawData;
}

void VertexData::initVertexAttributes(unsigned VAO) 
{
	// Calculate offsets.  (These should be static)
	static int colOffset = sizeof(*VertexData::position);
	static int outlineOffset = sizeof(*VertexData::position) + sizeof(*VertexData::color);
	static int idOffset = sizeof(*VertexData::position) + sizeof(*VertexData::color) + sizeof(*VertexData::outline);
	// Position.
	GLCall(glEnableVertexArrayAttrib(VAO, 0));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, totalSizeVD, (const void*)0));
	// color.
	GLCall(glEnableVertexArrayAttrib(VAO, 1));
	GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, totalSizeVD, (const void*)colOffset));
	// Outline.
	GLCall(glEnableVertexArrayAttrib(VAO, 2));
	GLCall(glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, totalSizeVD, (const void*)outlineOffset));
	// Entity ID.
	GLCall(glEnableVertexArrayAttrib(VAO, 3));
	GLCall(glVertexAttribIPointer(3, 1, GL_UNSIGNED_INT, totalSizeVD, (const void*)idOffset));
}

int VertexData::getTotalSize() { return totalSizeVD; };
int VertexData::getDataSize()  { return dataSizeVD;  };
int VertexData::getIDOffset()  { return idOffsetVD;  };
int VertexData::getIDSize()	   { return idSizeVD;	 };

//=============================================================================================================================================//
//  Vertex Data Textured.																													   //
//=============================================================================================================================================//

VertexDataTextured::VertexDataTextured(const glm::vec3& pos, const glm::vec4& colorIn, const glm::vec2& texCoords, float texID, unsigned int eID)
{
	*position = pos; *color = colorIn; entityID = eID; *textureCoords = texCoords; *textureID = texID; entityID = eID; *outline = 0.f;
}

const void* VertexDataTextured::getData()
{
	return (const void*)rawData;
}

void VertexDataTextured::initVertexAttributes(unsigned VAO)
{
	static int vertexSize = sizeof(*VertexDataTextured::position) + sizeof(*VertexDataTextured::color) + sizeof(*VertexDataTextured::textureCoords) +
							sizeof(*VertexDataTextured::textureID) + sizeof(*VertexDataTextured::outline) + sizeof(VertexDataTextured::entityID);
	int offset = 0;
	// Position.
	GLCall(glEnableVertexArrayAttrib(VAO, 0));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
	offset += sizeof(*VertexDataTextured::position);
	// Bind Vertex color attribute.
	GLCall(glEnableVertexArrayAttrib(VAO, 1));
	GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
	offset += sizeof(*VertexDataTextured::color);
	// Bind texture position attribute
	GLCall(glEnableVertexArrayAttrib(VAO, 2));
	GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
	offset += sizeof(*VertexDataTextured::textureCoords);
	// Outline value.
	GLCall(glEnableVertexArrayAttrib(VAO, 3));
	GLCall(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
	offset += sizeof(*VertexDataTextured::outline);
	// Bind texture ID attribute
	GLCall(glEnableVertexArrayAttrib(VAO, 4));
	GLCall(glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
	offset += sizeof(*VertexDataTextured::textureID);
	// Entity ID.
	GLCall(glEnableVertexArrayAttrib(VAO, 5));
	GLCall(glVertexAttribIPointer(5, 1, GL_UNSIGNED_INT, vertexSize, (const void*)offset));
}

int VertexDataTextured::getTotalSize() { return totalSizeVDT; };
int VertexDataTextured::getDataSize()  { return dataSizeVDT;  };
int VertexDataTextured::getIDOffset()  { return idOffsetVDT;  };
int VertexDataTextured::getIDSize()    { return idSizeVDT;	  };

//=============================================================================================================================================//
//  Vertex Data Circle.																														   //
//=============================================================================================================================================//

VertexDataCircle::VertexDataCircle(const glm::vec3& Position, const glm::vec2& localCoord, const glm::vec4& Colour, float Thickness, float Fade, unsigned int eID)
{
	*position = Position; 
	*localCoords = localCoord;
	*color = Colour;
	entityID = eID;
	*thickness = Thickness;
	*fade = Fade;
	*outline = 0.f;
}

const void* VertexDataCircle::getData()
{
	return (const void*)rawData;
}

void VertexDataCircle::initVertexAttributes(unsigned VAO)
{
	static int vertexSize = sizeof(*VertexDataCircle::position) + sizeof(*VertexDataCircle::color) + sizeof(*VertexDataCircle::fade) +
							sizeof(*VertexDataCircle::thickness) + sizeof(VertexDataCircle::entityID) + sizeof(*VertexDataCircle::localCoords) +
							sizeof(*VertexDataCircle::outline);
	int offset = 0;
	// Position.
	GLCall(glEnableVertexArrayAttrib(VAO, 0));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
	offset += sizeof(*VertexDataCircle::position);
	// Bind Vertex color attribute.
	GLCall(glEnableVertexArrayAttrib(VAO, 1));
	GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
	offset += sizeof(*VertexDataCircle::color);
	// Local coordinates.
	GLCall(glEnableVertexArrayAttrib(VAO, 2));
	GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
	offset += sizeof(*VertexDataCircle::localCoords);
	// Outline value.
	GLCall(glEnableVertexArrayAttrib(VAO, 3));
	GLCall(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
	offset += sizeof(*VertexDataCircle::outline);
	// Circle thickness.
	GLCall(glEnableVertexArrayAttrib(VAO, 4));
	GLCall(glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
	offset += sizeof(*VertexDataCircle::thickness);
	// Circle fade.
	GLCall(glEnableVertexArrayAttrib(VAO, 5));
	GLCall(glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
	offset += sizeof(*VertexDataCircle::fade);
	// Entity ID.
	GLCall(glEnableVertexArrayAttrib(VAO, 6));
	GLCall(glVertexAttribIPointer(6, 1, GL_UNSIGNED_INT, vertexSize, (const void*)offset));
}

int VertexDataCircle::getTotalSize() { return totalSizeVDC; };
int VertexDataCircle::getDataSize()  { return dataSizeVDC;  };
int VertexDataCircle::getIDOffset()  { return idOffsetVDC;  };
int VertexDataCircle::getIDSize()    { return idSizeVDC;    };

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//