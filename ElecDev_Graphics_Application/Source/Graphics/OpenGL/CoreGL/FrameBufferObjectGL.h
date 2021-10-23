#pragma once

/*
These buffers are used to render OpenGL to a texture that can be used in various ways.
It is going to be used to render to an ImGUI widget.
[https://learnopengl.com/Advanced-OpenGL/Framebuffers]
*/

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include "ErrorHandlerGL.h"
#include <glad/glad.h>

//----------------------------------------------------------------------------------------------------------------------
//  Class.
//----------------------------------------------------------------------------------------------------------------------

class FrameBufferObject 
{

private:

	// FBO that will be rendered.
	unsigned int m_renderFrameBufferID;			// FBO ID.
	unsigned int m_renderColorTextureID;		// Texture ID.	

	// MSAA FBO.
	unsigned int m_msaaFrameBufferID;			// FBO ID.
	unsigned int m_msaaColorTextureID;			// Texture ID.	
	unsigned int m_msaaDepthStencilBufferID;	// Depth/Stencil buffer.

	// Saves the FBO size.
	int m_viewport[2] = { 500, 500 };

	int m_MSAA = 1;

public:

	// Constructor.
	FrameBufferObject(int width, int height, int MSAA);
	// Destructor.
	~FrameBufferObject();
	
	// Bind the FBO.
	void bind();
	// Unbind the FBO.
	void unbind();
	// Create the color, depth and stencil attachments.
	void createAttachments(int width, int height);
	// Get the texture ID.
	unsigned int getRenderTexture();

	// Resizing the texure when the window resizes.
	void resize(int width, int height);

	// Clears the FBO.
	void clear();

};

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------