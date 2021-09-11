/*
Texture handling for the Base Engine.
*/

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include "baseEngineCore.h"
#include "Peripherals/stb_image.h"

//----------------------------------------------------------------------------------------------------------------------
//  Text rendering functions.
//----------------------------------------------------------------------------------------------------------------------

// Function that loads a texture from the file path.
GLuint BaseEngineGL::loadTexture(const std::string& path, bool alpha) 
{
	int w, h, bits;
	stbi_set_flip_vertically_on_load(1);

	// Load texture without alpha channel.
	if (!alpha) 
	{
		auto* pixels = stbi_load(path.c_str(), &w, &h, &bits, STBI_rgb);
		GLuint textureID;
		GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &textureID));
		GLCall(glBindTexture(GL_TEXTURE_2D, textureID));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels));
		stbi_image_free(pixels);
		return textureID;
	}
	// Load texture with alpha channel.
	else
	{
		auto* pixels = stbi_load(path.c_str(), &w, &h, &bits, STBI_rgb_alpha);
		GLuint textureID;
		GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &textureID));
		GLCall(glBindTexture(GL_TEXTURE_2D, textureID));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels));
		stbi_image_free(pixels);
		return textureID;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------