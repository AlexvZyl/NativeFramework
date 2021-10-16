//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

// Class include.
#include "frameBufferObject.h"

//----------------------------------------------------------------------------------------------------------------------
// Functions. 
//----------------------------------------------------------------------------------------------------------------------

// Constructor.
FrameBufferObject::FrameBufferObject(int width, int height) 
{
	// Generate the frame buffer.
	GLCall(glGenFramebuffers(1, &m_rendererID));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID));
	// Check for error.
	if (!glCheckFramebufferStatus(GL_FRAMEBUFFER)) 
	{
		std::cout << "[BASE_ENGINE][ERROR] FBO could not be generated.\n\n";
	}

	// Generate a texture for the frame buffer.
	GLCall(glGenTextures(1, &m_textureID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_textureID));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	// Attach texture to the frame buffer.
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureID, 0));

	// Check for failure.
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "[BASE_ENGINE][ERROR] FBO is not complete!\n\n" << std::endl;
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

// Destructor.
FrameBufferObject::~FrameBufferObject()
{
	// Delete the frame buffer.
	GLCall(glDeleteFramebuffers(1, &m_rendererID));
}

// Bind the FBO.
void FrameBufferObject::bind() 
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID));
	GLCall(glEnable(GL_DEPTH_TEST));
}

// Clear the FBO.
void FrameBufferObject::clear() 
{
	GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

// Unbind the FBO.
void FrameBufferObject::unbind() 
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

// Get access to the texture ID.
unsigned int FrameBufferObject::getTexID() 
{
	return m_textureID;
}

// Resizing the texture for when the window changes size.
void FrameBufferObject::resize(int width, int height) 
{
	bind();
	// Delete texture.
	GLCall(glDeleteTextures(1, &m_textureID));
	// Generate a texture for the frame buffer.
	GLCall(glGenTextures(1, &m_textureID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_textureID));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	// Attach texture to the frame buffer.
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureID, 0));
	unbind();
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------