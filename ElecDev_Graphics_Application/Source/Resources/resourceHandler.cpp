/*=======================================================================================================================================*/
/* Includes                                                                                                                              */
/*=======================================================================================================================================*/

// General.
#include "resource.h"
#include <iostream>

// OpenGL.
#include <ErrorHandler/errorHandler.h>

/*=======================================================================================================================================*/
/* Functions.                                                                                                                            */
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

// Loads a bitmap from the executable and saves a txture ID.
Bitmap loadBitmapFromResource(int bitmapID)
{
	// Bitmap to return.
	Bitmap bitmap;

	// Load bitmap handle from exe.
	HBITMAP winBitmapHandle = (HBITMAP)LoadImage(getCurrentModule(), MAKEINTRESOURCE(bitmapID), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	// Convert to bitmap data.
	BITMAP winBitmap;
	GetObject(winBitmapHandle, sizeof(winBitmap), &winBitmap);

	// Store in local format and return.
	bitmap.width = winBitmap.bmWidth;
	bitmap.height = winBitmap.bmHeight;
	bitmap.pixelData = winBitmap.bmBits;
	return bitmap;


	//// Create windows bitmap.
	//HBITMAP hBmp = (HBITMAP)LoadImage(getCurrentModule(), MAKEINTRESOURCE(bitmapID), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
	//std::cout << GetLastError() << "\n";
	//// Get info about the bitmap
	//

	//HBITMAP bitmapHandle1 = reinterpret_cast<HBITMAP>(LoadImage(GetModuleHandle(nullptr), MAKEINTRESOURCE(bitmapID), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE));
	//HBITMAP bitmapHandle2 = LoadBitmapA(getCurrentModule(), MAKEINTRESOURCEA(ARIAL_SDF_BMP));
	//HBITMAP bitmapHandle3 = LoadBitmap((HINSTANCE)getCurrentModule(), MAKEINTRESOURCE(bitmapID));
	//HBITMAP bitmapHandle4 = (HBITMAP)::LoadImage(getCurrentModule(), MAKEINTRESOURCE(bitmapID), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
	//HBITMAP bitmapHandle5 = reinterpret_cast<HBITMAP>(LoadImage(getCurrentModule(), MAKEINTRESOURCE(bitmapID), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE));
	//BITMAP bitmapData;
	//int error = GetObject(bitmapHandle1, sizeof(BITMAP), &bitmapData);

	// Return the bitmap data.

}

// Loads a bitmap into OpenGL and returns the texture ID.
unsigned int loadBitmapToGL(Bitmap bitmap) 
{
	unsigned int textureID = 0;
	GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &textureID));
	GLCall(glBindTexture(GL_TEXTURE_2D, textureID));
	GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 4));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap.width, bitmap.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (const void*)bitmap.pixelData));

	//GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmapData.bmWidth, bitmapData.bmHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmapData.bmBits));
	//stbi_image_free(pixels);
	//DeleteObject((HGDIOBJ)bitmapHandle1);

	// Tell OpenGL to ignore padding at ends of rows.
	//GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (const void*)bits);

	// Return OpenGL texture ID.
	return textureID;
}

/*=======================================================================================================================================*/
/* EOF.                                                                                                                                  */
/*=======================================================================================================================================*/