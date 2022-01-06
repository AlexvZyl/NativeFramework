//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "CoreGL/Entities/Vertex.h"

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

Vertex::Vertex() { }

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

const void* VertexData::dataGL()
{
	return (const void*)rawData;
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

const void* VertexDataTextured::dataGL()
{
	return (const void*)rawData;
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

const void* VertexDataCircle::dataGL()
{
	return (const void*)rawData;
}

int VertexDataCircle::getTotalSize() { return totalSizeVDC; };
int VertexDataCircle::getDataSize()  { return dataSizeVDC;  };
int VertexDataCircle::getIDOffset()  { return idOffsetVDC;  };
int VertexDataCircle::getIDSize()    { return idSizeVDC;    };

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//