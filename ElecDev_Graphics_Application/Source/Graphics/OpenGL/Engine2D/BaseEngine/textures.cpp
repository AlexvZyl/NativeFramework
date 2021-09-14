/*
Texture handling for the Base Engine.
*/

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include "baseEngineCore.h"
#include "Peripherals/stb_image.h"

// Loading from executable.
#include <Windows.h>
#include "../Resources/resource.h"

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
		int w, h;
		auto* pixels = stbi_load(path.c_str(), &w, &h, &bits, STBI_rgb_alpha);
		GLuint textureID;
		GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &textureID));
		GLCall(glBindTexture(GL_TEXTURE_2D, textureID));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pixels));
		// Clear memory.
		DeleteObject(pixels);
		return textureID;
	}
}

//----------------------------------------------------------------------------------------------------------------------

GLuint BaseEngineGL::loadBMPtoGL(HBITMAP bitmap_handle)
{
    const int BytesPerPixel = sizeof(DWORD);
    HDC context = GetDC(NULL);

    SIZE bitmap_size;
    if (!GetBitmapDimensionEx(bitmap_handle, &bitmap_size))
        return 0;
    size_t bitmap_buffer_size = bitmap_size.cx * bitmap_size.cy * BytesPerPixel;
    void* bitmap_buffer;
    bitmap_buffer = malloc(bitmap_buffer_size);
    // Test if failed.
    if (!bitmap_buffer)
        return 0;

    BITMAPINFO bitmap_info;
    memset(&bitmap_info, 0, sizeof(bitmap_info));
    bitmap_info.bmiHeader.biSize = sizeof(bitmap_info.bmiHeader);
    bitmap_info.bmiHeader.biWidth = bitmap_size.cx;
    bitmap_info.bmiHeader.biHeight = bitmap_size.cy;
    bitmap_info.bmiHeader.biPlanes = 1;
    bitmap_info.bmiHeader.biBitCount = 8;
    bitmap_info.bmiHeader.biCompression = BI_RGB;

    // Check for failure.
    if (!GetDIBits(context,
                   bitmap_handle,
                   0, bitmap_size.cy,
                   bitmap_buffer,
                   &bitmap_info,
                   DIB_RGB_COLORS))
    {
        free(bitmap_buffer);
        return 0;
    }

    // Load into OpenGL.
    GLuint texture_name;
    glGenTextures(1, &texture_name);
    glBindTexture(GL_TEXTURE_2D, texture_name);
    glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
    glPixelStorei(GL_UNPACK_LSB_FIRST, GL_TRUE);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 bitmap_size.cx, bitmap_size.cy, 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 bitmap_buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Clear memory.
    free(bitmap_buffer);
    // Return Texture ID.
    return texture_name;
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------