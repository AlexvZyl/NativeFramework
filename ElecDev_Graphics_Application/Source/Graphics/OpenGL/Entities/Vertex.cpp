//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "OpenGL/Entities/Vertex.h"
#include "External/GLAD/Includes/glad/glad.h"
#include "OpenGL/ErrorHandlerGL.h"

//=============================================================================================================================================//
//  Static values.																															   //
//=============================================================================================================================================//

// VertexData.
int VertexData::idSizeVD					= sizeof(VertexData::entityID);
int VertexData::dataSizeVD					= sizeof(*VertexData::position) + sizeof(*VertexData::color);
int VertexData::idOffsetVD					= VertexData::dataSizeVD;
int VertexData::totalSizeVD					= VertexData::dataSizeVD + VertexData::idSizeVD;									

// VertexDataTextured.
int VertexDataTextured::idSizeVDT			= sizeof(VertexDataTextured::entityID);
int VertexDataTextured::dataSizeVDT			= sizeof(*VertexDataTextured::position) + sizeof(*VertexDataTextured::color)
											+ sizeof(*VertexDataTextured::textureCoords) + sizeof(*VertexDataTextured::textureID);
int VertexDataTextured::idOffsetVDT			= VertexDataTextured::dataSizeVDT;
int VertexDataTextured::totalSizeVDT		= VertexDataTextured::dataSizeVDT + VertexDataTextured::idSizeVDT;

// Vertex Data Circle.
int VertexDataCircle::idSizeVDC				= sizeof(VertexDataCircle::entityID);
int VertexDataCircle::dataSizeVDC			= sizeof(*VertexDataCircle::position) + sizeof(*VertexDataCircle::color) + sizeof(*VertexDataCircle::localCoords)
											+ sizeof(*VertexDataCircle::fade) + sizeof(*VertexDataCircle::fade);
int VertexDataCircle::idOffsetVDC			= VertexDataCircle::dataSizeVDC;
int VertexDataCircle::totalSizeVDC			= VertexDataCircle::dataSizeVDC + VertexDataCircle::idSizeVDC;

//=============================================================================================================================================//
//  Vertex Data.																															   //
//=============================================================================================================================================//

VertexData::VertexData(glm::vec3& pos, glm::vec4& clr, unsigned int eID)
{
	*position = pos; *color = clr; entityID = eID;
}

VertexData::VertexData(float pos0, float pos1, float pos2,
					   float col0, float col1, float col2, float col3,
					   unsigned int eID)
{
	// Assign data.
	position->x = pos0; position->y = pos1; position->z = pos2;
	color->r = col0;
	color->g = col1;
	color->b = col2;
	color->a = col3;
	entityID = eID;
}

const void* VertexData::getData()
{
	return (const void*)rawData;
}

void VertexData::initVertexAttributes(unsigned vao) 
{
	// Calculate offsets.  (These should be static)
	int colOffset = sizeof(*VertexData::position);
	int idOffset = sizeof(*VertexData::position) + sizeof(*VertexData::color);
	// Position.
	GLCall(glEnableVertexArrayAttrib(vao, 0));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, totalSizeVD, (const void*)0));
	// color.
	GLCall(glEnableVertexArrayAttrib(vao, 1));
	GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, totalSizeVD, (const void*)colOffset));
	// Entity ID.
	GLCall(glEnableVertexArrayAttrib(vao, 2));
	GLCall(glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, totalSizeVD, (const void*)idOffset));
}

int VertexData::getTotalSize() { return totalSizeVD; };
int VertexData::getDataSize()  { return dataSizeVD;  };
int VertexData::getIDOffset()  { return idOffsetVD;  };
int VertexData::getIDSize()	   { return idSizeVD;	 };

//=============================================================================================================================================//
//  Vertex Data Textured.																													   //
//=============================================================================================================================================//

VertexDataTextured::VertexDataTextured(glm::vec3& pos, glm::vec4& clr, glm::vec2& texCoords, float texID, unsigned int eID)
{
	*position = pos; *color = clr; entityID = eID; *textureCoords = texCoords; *textureID = texID; entityID = eID;
}

VertexDataTextured::VertexDataTextured(float pos0, float pos1, float pos2,
	float col0, float col1, float col2, float col3,
	float texPos0, float texPos1, float texID,
	unsigned int eID)
{
	// Assign data.
	position->x = pos0; position->y = pos1; position->z = pos2;
	color->r = col0; color->g = col1; color->b = col2; color->a = col3;
	entityID = eID;
	textureCoords->x = texPos0; textureCoords->y = texPos1;
	*textureID = (float)texID;
}

const void* VertexDataTextured::getData()
{
	return (const void*)rawData;
}

void VertexDataTextured::initVertexAttributes(unsigned vao)
{
	int vertexSize = sizeof(*VertexDataTextured::position) + sizeof(*VertexDataTextured::color) + sizeof(*VertexDataTextured::textureCoords) +
		sizeof(*VertexDataTextured::textureID) + sizeof(VertexDataTextured::entityID);
	int offset = 0;
	// Position.
	GLCall(glEnableVertexArrayAttrib(vao, 0));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
	offset += sizeof(*VertexDataTextured::position);
	// Bind Vertex color attribute.
	GLCall(glEnableVertexArrayAttrib(vao, 1));
	GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
	offset += sizeof(*VertexDataTextured::color);
	// Bind texture position attribute
	GLCall(glEnableVertexArrayAttrib(vao, 2));
	GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
	offset += sizeof(*VertexDataTextured::textureCoords);
	// Bind texture ID attribute
	GLCall(glEnableVertexArrayAttrib(vao, 3));
	GLCall(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
	offset += sizeof(*VertexDataTextured::textureID);
	// Entity ID.
	GLCall(glEnableVertexArrayAttrib(vao, 4));
	GLCall(glVertexAttribIPointer(4, 1, GL_UNSIGNED_INT, vertexSize, (const void*)offset));
}

int VertexDataTextured::getTotalSize() { return totalSizeVDT; };
int VertexDataTextured::getDataSize()  { return dataSizeVDT;  };
int VertexDataTextured::getIDOffset()  { return idOffsetVDT;  };
int VertexDataTextured::getIDSize()    { return idSizeVDT;	  };

//=============================================================================================================================================//
//  Vertex Data Circle.																														   //
//=============================================================================================================================================//

VertexDataCircle::VertexDataCircle(glm::vec3& Position, glm::vec2& localCoord, glm::vec4& Colour, float Thickness, float Fade, unsigned int eID)
{
	*position = Position; 
	*localCoords = localCoord;
	*color = Colour;
	entityID = eID;
	*thickness = Thickness;
	*fade = Fade;
}

const void* VertexDataCircle::getData()
{
	return (const void*)rawData;
}

void VertexDataCircle::initVertexAttributes(unsigned vao)
{
	int vertexSize = sizeof(*VertexDataCircle::position) + sizeof(*VertexDataCircle::color) + sizeof(*VertexDataCircle::fade) +
					 sizeof(*VertexDataCircle::thickness) + sizeof(VertexDataCircle::entityID) + sizeof(*VertexDataCircle::localCoords);
	int offset = 0;
	// Position.
	GLCall(glEnableVertexArrayAttrib(vao, 0));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
	offset += sizeof(*VertexDataCircle::position);
	// Bind Vertex color attribute.
	GLCall(glEnableVertexArrayAttrib(vao, 1));
	GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
	offset += sizeof(*VertexDataCircle::color);
	// Local coordinates.
	GLCall(glEnableVertexArrayAttrib(vao, 2));
	GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
	offset += sizeof(*VertexDataCircle::localCoords);
	// Circle thickness.
	GLCall(glEnableVertexArrayAttrib(vao, 3));
	GLCall(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
	offset += sizeof(*VertexDataCircle::thickness);
	// Circle fade.
	GLCall(glEnableVertexArrayAttrib(vao, 4));
	GLCall(glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, vertexSize, (const void*)offset));
	offset += sizeof(*VertexDataCircle::fade);
	// Entity ID.
	GLCall(glEnableVertexArrayAttrib(vao, 5));
	GLCall(glVertexAttribIPointer(5, 1, GL_UNSIGNED_INT, vertexSize, (const void*)offset));
}

int VertexDataCircle::getTotalSize() { return totalSizeVDC; };
int VertexDataCircle::getDataSize()  { return dataSizeVDC;  };
int VertexDataCircle::getIDOffset()  { return idOffsetVDC;  };
int VertexDataCircle::getIDSize()    { return idSizeVDC;    };

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//