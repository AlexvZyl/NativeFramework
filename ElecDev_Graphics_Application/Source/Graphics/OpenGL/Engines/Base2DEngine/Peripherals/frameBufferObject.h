#pragma once

/*
These buffers are used to render OpenGL to a texture that can be used in various ways.
It is going to be used to render to an ImGUI widget.
[https://learnopengl.com/Advanced-OpenGL/Framebuffers]
*/

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include <ErrorHandler/ErrorHandler.h>
#include <glad/glad.h>

//----------------------------------------------------------------------------------------------------------------------
//  Class.
//----------------------------------------------------------------------------------------------------------------------

class FrameBufferObject 
{
	// Details regarding the FBO.
	struct FrameBufferSpecification 
	{
		
	};

private:

	// FBO id.
	unsigned int m_rendererID;
	// Texture ID.
	unsigned int m_textureID;

public:

	// Constructor.
	FrameBufferObject(int width, int height);
	// Destructor.
	~FrameBufferObject();
	
	// Bind the FBO.
	void bind();
	// Unbind the FBO.
	void unbind();
	// G>et the texture ID.
	unsigned int getTexID();

	// Resizing the texure when the window resizes.
	void resize(int width, int height);

	// Clears the FBO.
	void clear();

};

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------