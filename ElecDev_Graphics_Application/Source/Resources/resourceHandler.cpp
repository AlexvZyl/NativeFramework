/*=======================================================================================================================================*/
/* Includes                                                                                                                              */
/*=======================================================================================================================================*/

// General.
#include "ResourceHandler.h"
#include <iostream>
#include "glad/glad.h"
// Loading from resource.
#include <gdiplus.h>
#include <Shlwapi.h>
#pragma comment(lib,"gdiplus.lib")
#pragma comment(lib, "Shlwapi.lib")

// OpenGL error handler.
#include <Graphics/OpenGL/CoreGL/ErrorHandlerGL.h>

/*=======================================================================================================================================*/
/* Module.	                                                                                                                             */
/*=======================================================================================================================================*/

// Return the current module for resource loading (from the .exe).
HMODULE getCurrentModule()
{
	HMODULE hModule = NULL;
	GetModuleHandleEx(
		GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
		(LPCTSTR)getCurrentModule,
		&hModule);
	return hModule;
}

/*=======================================================================================================================================*/
/* Textfiles.	                                                                                                                             */
/*=======================================================================================================================================*/

// Loads the shader text file from the executable and returns it as a string.
std::string loadTextFromResource(int textID) 
{
	// Load resource from executable.
	HRSRC shaderResource = FindResource(getCurrentModule(), MAKEINTRESOURCE(textID), MAKEINTRESOURCE(TEXTFILE));
	HGLOBAL resourceData = LoadResource(getCurrentModule(), shaderResource);
	DWORD resourceSize = SizeofResource(getCurrentModule(), shaderResource);
	char* resourceFinal = (char*)LockResource(resourceData);
	std::string shaderSource;
	shaderSource.assign(resourceFinal, resourceSize);
	return shaderSource;
}

/*=======================================================================================================================================*/
/* Images.	                                                                                                                             */
/*=======================================================================================================================================*/

BITMAP loadImageFromResource(int resourceID)
{
	HBITMAP hbitmap = NULL;
	ULONG_PTR token;
	Gdiplus::GdiplusStartupInput tmp;
	Gdiplus::GdiplusStartup(&token, &tmp, NULL);
	if (auto hres = FindResource(getCurrentModule(), MAKEINTRESOURCE(resourceID), RT_RCDATA))
		if (auto size = SizeofResource(getCurrentModule(), hres))
			if (auto data = LockResource(LoadResource(getCurrentModule(), hres)))
				if (auto stream =  SHCreateMemStream((BYTE*)data, size))
				{
					Gdiplus::Bitmap bmp(stream);
					stream->Release();
					bmp.GetHBITMAP(Gdiplus::Color::Transparent, &hbitmap);
				}
	Gdiplus::GdiplusShutdown(token);
	BITMAP bitmap;
	GetObject(hbitmap, sizeof(BITMAP), &bitmap);
	return bitmap;
}

// Loads a bitmap into OpenGL and returns the texture ID.
unsigned int loadBitmapToGL(BITMAP bitmap) 
{
	unsigned int textureID = 0;
	GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &textureID));
	GLCall(glBindTexture(GL_TEXTURE_2D, textureID));
	GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 4));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap.bmWidth, bitmap.bmHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, (const void*)bitmap.bmBits));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	return textureID;
}

/*=======================================================================================================================================*/
/* EOF.                                                                                                                                  */
/*=======================================================================================================================================*/