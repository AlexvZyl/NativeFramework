//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Vertex.h"

//=============================================================================================================================================//
//  Static values.																															   //
//=============================================================================================================================================//

// VertexData.
int VertexData::idSizeVD					= sizeof(VertexData::entityID);
int VertexData::dataSizeVD					= sizeof(VertexData::data.position) + sizeof(VertexData::data.color);
int VertexData::idOffsetVD					= VertexData::dataSizeVD;
int VertexData::totalSizeVD					= VertexData::dataSizeVD + VertexData::idSizeVD;									

// VertexDataTextured.
int VertexDataTextured::idSizeVDT			= sizeof(VertexDataTextured::entityID);
int VertexDataTextured::dataSizeVDT			= sizeof(VertexDataTextured::data.position) + sizeof(VertexDataTextured::data.color)
											+ sizeof(VertexDataTextured::data.textureCoords) + sizeof(VertexDataTextured::data.textureID);
int VertexDataTextured::idOffsetVDT			= VertexDataTextured::dataSizeVDT;
int VertexDataTextured::totalSizeVDT		= VertexDataTextured::dataSizeVDT + VertexDataTextured::idSizeVDT;

// Vertex Data Circle.
int VertexDataCircle::idSizeVDC				= sizeof(VertexDataCircle::entityID);
int VertexDataCircle::dataSizeVDC			= sizeof(VertexDataCircle::data.position) + sizeof(VertexDataCircle::data.color)
											+ sizeof(VertexDataCircle::localCoords);
int VertexDataCircle::idOffsetVDC			= VertexDataCircle::dataSizeVDC;
int VertexDataCircle::totalSizeVDC			= VertexDataCircle::dataSizeVDC + VertexDataCircle::idSizeVDC;


//=============================================================================================================================================//
//  Vertex Data.																															   //
//=============================================================================================================================================//

// Constructor.
VertexData::VertexData() {}

VertexData::VertexData(glm::vec3* pos, glm::vec4* clr, unsigned int eID)
{
	data.position = *pos; data.color = *clr; entityID = eID;
}

VertexData::VertexData(float pos0, float pos1, float pos2,
	float col0, float col1, float col2, float col3,
	unsigned int eID)
{
	// Assign position.
	data.position.x = pos0; data.position.y = pos1; data.position.z = pos2;
	// Assign color.
	data.color.r = col0;
	data.color.g = col1;
	data.color.b = col2;
	data.color.a = col3;
	// Assign ID.
	entityID = eID;
}

const void* VertexData::dataGL()
{
	return (const void*)rawData;
}

// Vertex specific data.
int VertexData::getTotalSize() { return totalSizeVD; };
int VertexData::getDataSize()  { return dataSizeVD;  };
int VertexData::getIDOffset()  { return idOffsetVD;  };
int VertexData::getIDSize()	   { return idSizeVD;	 };

//=============================================================================================================================================//
//  Vertex Data Textured.																													   //
//=============================================================================================================================================//

// Constructors.
VertexDataTextured::VertexDataTextured() {}

VertexDataTextured::VertexDataTextured(glm::vec3* pos, glm::vec4* clr, glm::vec2* texCoords, float texID, unsigned int eID)
{
	data.position = *pos; data.color = *clr; entityID = eID; data.textureCoords = *texCoords; data.textureID = texID; entityID = eID;
}

VertexDataTextured::VertexDataTextured(float pos0, float pos1, float pos2,
	float col0, float col1, float col2, float col3,
	float texPos0, float texPos1, float texID,
	unsigned int eID)
{
	// Assign position.
	data.position.x = pos0; data.position.y = pos1; data.position.z = pos2;
	// Assign color.
	data.color.r = col0; data.color.g = col1; data.color.b = col2; data.color.a = col3;
	// Assign entity ID.
	entityID = eID;
	// Assign texture ID position.
	data.textureCoords.x = texPos0; data.textureCoords.y = texPos1;
	// Assigne texture ID.
	data.textureID = (unsigned int)texID;
}

// Return the raw data for OpenGL to use.
const void* VertexDataTextured::dataGL()
{
	return (const void*)rawData;
}

// Vertex specific data.
int VertexDataTextured::getTotalSize() { return totalSizeVDT; };
int VertexDataTextured::getDataSize()  { return dataSizeVDT;  };
int VertexDataTextured::getIDOffset()  { return idOffsetVDT;  };
int VertexDataTextured::getIDSize()    { return idSizeVDT;	  };

//=============================================================================================================================================//
//  Vertex Data Circle.																														   //
//=============================================================================================================================================//

// Constructors.
VertexDataCircle::VertexDataCircle() {}

VertexDataCircle::VertexDataCircle(glm::vec3 pos, glm::vec4 clr, glm::vec2 lclCoords, unsigned int eID)
{
	data.position = pos; data.color = clr; entityID = eID; localCoords = lclCoords;
}


const void* VertexDataCircle::dataGL()
{
	return (const void*)rawData;
}

// Vertex specific data.
int VertexDataCircle::getTotalSize() { return totalSizeVDC; };
int VertexDataCircle::getDataSize()  { return dataSizeVDC;  };
int VertexDataCircle::getIDOffset()  { return idOffsetVDC;  };
int VertexDataCircle::getIDSize()    { return idSizeVDC;    };

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//