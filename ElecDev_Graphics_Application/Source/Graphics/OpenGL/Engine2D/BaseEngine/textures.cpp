/*
Texture handling for the Base Engine.
*/

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

// Loading from executable.
#include <Windows.h>
#include "Misc/stb_image.h"

#include "baseEngineCore.h"
#include "Resources/resource.h"

//----------------------------------------------------------------------------------------------------------------------
//  Texture loading..
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

GLuint BaseEngineGL::loadBMPtoGL(int bitmapID)
{
	// Load BMP from file.
	//int width, height, bits;
	//stbi_set_flip_vertically_on_load(1);
	//auto* pixels = stbi_load("Source\\Resources\\Fonts\\Arial_SDF_BMP32.bmp", &width, &height, &bits, STBI_rgb_alpha);

	// Create windows bitmap.
	HBITMAP hBmp = (HBITMAP) LoadImage(getCurrentModule(), MAKEINTRESOURCE(bitmapID), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
	std::cout << GetLastError() << "\n";
	// Get info about the bitmap
	BITMAP BM;
	GetObject(hBmp, sizeof(BM), &BM);

	HBITMAP bitmapHandle1 = reinterpret_cast<HBITMAP>(LoadImage(GetModuleHandle(nullptr), MAKEINTRESOURCE(bitmapID), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE));
	HBITMAP bitmapHandle2 = LoadBitmapA(getCurrentModule(), MAKEINTRESOURCEA(ARIAL_SDF_BMP));
	HBITMAP bitmapHandle3 = LoadBitmap((HINSTANCE)getCurrentModule(), MAKEINTRESOURCE(bitmapID));
	HBITMAP bitmapHandle4 = (HBITMAP)LoadImage(getCurrentModule(), MAKEINTRESOURCE(bitmapID), IMAGE_BITMAP, 0, 0, 0);
	HBITMAP bitmapHandle5 = reinterpret_cast<HBITMAP>(LoadImage(getCurrentModule(), MAKEINTRESOURCE(bitmapID), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE));
	BITMAP bitmapData;
	int error = GetObject(bitmapHandle1, sizeof(BITMAP), &bitmapData);

	GLuint textureID=0;
	GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &textureID));
	GLCall(glBindTexture(GL_TEXTURE_2D, textureID));
	GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 4));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	//GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (const void*)pixels));

	//GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmapData.bmWidth, bitmapData.bmHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmapData.bmBits));
	//stbi_image_free(pixels);
	//DeleteObject((HGDIOBJ)bitmapHandle1);
	return textureID;

	// Tell OpenGL to ignore padding at ends of rows.
	//GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (const void*)bits);

	//// Return OpenGL texture ID.
	//return textureID;
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------