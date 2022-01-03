#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//



//==============================================================================================================================================//
//  Texture Class.																																//
//==============================================================================================================================================//

class Texture 
{
public:

	// Constructor.
	Texture(int resourceID);

	// Sert the mipmap level for the texture.
	void setMipmapLevel(float LOD);

private:

	unsigned m_GLID;
	unsigned m_textureType;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//