//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "OpenGL/ErrorHandlerGL.h"
#include "glad/glad.h"
#include "OpenGL/Primitives/TextureGL.h"
#include "Resources/ResourceHandler.h"

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

Texture::Texture(int resourceID, const std::string& name) 
	: m_name(name)
{
	m_rendererID = loadBitmapToGL(loadImageFromResource(resourceID));
	m_textureType = GL_TEXTURE_2D;
}

void Texture::setMipmapLevel(float LOD) 
{
	// Enable mipmapping.
	GLCall(glBindTexture(m_textureType, m_rendererID));
	GLCall(glGenerateMipmap(m_textureType));
	GLCall(glTexParameteri(m_textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GLCall(glTexParameterf(m_textureType, GL_TEXTURE_LOD_BIAS, LOD));
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//