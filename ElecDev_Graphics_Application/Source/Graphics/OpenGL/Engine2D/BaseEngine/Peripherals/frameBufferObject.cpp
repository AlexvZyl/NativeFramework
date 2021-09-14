//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

// Class include.
#include "frameBufferObject.h"

//----------------------------------------------------------------------------------------------------------------------
// Functions. 
//----------------------------------------------------------------------------------------------------------------------

// Constructor.
FrameBufferObject::FrameBufferObject() 
{
	// Generate the frame buffer.
	GLCall(glGenFramebuffers(1, &m_rendererID));
	glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);

	// Generate a texture for the frame buffer.
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Attach texture to the frame buffer.
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureID, 0);

	// Generate render buffer and attach it to the frame buffer.
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	// Check for failure.
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "[OPEMGL][ERROR] Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
	GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GLCall(glEnable(GL_DEPTH_TEST));
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

// Resizing for when the window changes size.
void FrameBufferObject::resize() 
{

}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------