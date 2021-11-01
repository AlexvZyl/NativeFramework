//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

// Class include.
#include "FrameBufferObjectGL.h"

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

// Constructor.
FrameBufferObject::FrameBufferObject(int width, int height, int MSAA) 
	: m_MSAA(MSAA)
{
	// Generate MSAA FBO.
	GLCall(glGenFramebuffers(1, &m_msaaFrameBufferID));
	// Check for generation error.
	if (!glCheckFramebufferStatus(GL_FRAMEBUFFER))
	{ std::cout << red << "\n\n[OPENGL] [ERROR] :" << white << " MSAA FBO could not be generated.\n"; }

	// Generate render FBO.
	GLCall(glGenFramebuffers(1, &m_renderFrameBufferID));
	// Check for generation error.
	if (!glCheckFramebufferStatus(GL_FRAMEBUFFER))
	{ std::cout << red << "\n\n[OPENGL] [ERROR] :" << white << " Render FBO could not be generated.\n"; }

	// Create attachments for the FBO's.
	createAttachments(width, height);

	// Save the dimenions.
	m_viewport[0] = width;
	m_viewport[1] = height;
}

// Create the FBO attachments.
void FrameBufferObject::createAttachments(int width, int height)
{
	// -------------------------------- //
	//  M S A A   A T T A C H M E N T S //
	// -------------------------------- //

	// Bind MSA FBO.
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_msaaFrameBufferID));
	// Generate and attach MSAA color texture.
	GLCall(glGenTextures(1, &m_msaaColorTextureID));
	GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_msaaColorTextureID));
	GLCall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_MSAA, GL_RGBA, width, height, GL_TRUE));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_msaaColorTextureID, 0));
	// Generate and attach MSAA entityID buffer.
	GLCall(glGenTextures(1, &m_msaaEntityIDBufferID));
	GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_msaaEntityIDBufferID));
	GLCall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_MSAA, GL_R32UI, width, height, GL_TRUE));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D_MULTISAMPLE, m_msaaEntityIDBufferID, 0));
	// Enable draw buffers.
	GLenum drawBuffers[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
	GLCall(glDrawBuffers(2, drawBuffers));
	// Create depth/stencil MSAA buffer texture.
	GLCall(glGenRenderbuffers(1, &m_msaaDepthStencilBufferID));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_msaaDepthStencilBufferID));
	GLCall(glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_MSAA, GL_DEPTH24_STENCIL8, width, height));
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_msaaDepthStencilBufferID));	
	// Check for completion errror.
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << red << "\n\n[OPENGL] [ERROR] :" << white << " MSAA FBO is not complete.\n" << std::endl;

	// ------------------------------------- //
	//  R E N D E R   A T T A C H M E N T S  //
	// ------------------------------------- //

	// Bind render FBO.
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_renderFrameBufferID));
	// Generate and attach color texture.
	GLCall(glGenTextures(1, &m_renderColorTextureID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_renderColorTextureID));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_renderColorTextureID, 0));
	// Generate Entity ID texture.
	GLCall(glGenTextures(1, &m_renderEntityIDBufferID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_renderEntityIDBufferID));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, NULL));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_renderEntityIDBufferID, 0));
	// Check for completion errror.
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << red << "\n\n[OPENGL] [ERROR] :" << white << " Render FBO is not complete.\n" << std::endl;

	// --------------- //
	//  C L E A N U P  //
	// --------------- //

	// Unbind.
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

// Destructor.
FrameBufferObject::~FrameBufferObject()
{
	// Delete the textures.
	GLCall(glDeleteTextures(1, &m_msaaColorTextureID));
	GLCall(glDeleteTextures(1, &m_msaaDepthStencilBufferID));
	GLCall(glDeleteTextures(1, &m_msaaEntityIDBufferID));
	GLCall(glDeleteTextures(1, &m_renderColorTextureID));
	GLCall(glDeleteTextures(1, &m_renderEntityIDBufferID));
	// Delete the buffers.
	GLCall(glDeleteFramebuffers(1, &m_msaaFrameBufferID));
	GLCall(glDeleteFramebuffers(1, &m_renderFrameBufferID));
}

//=============================================================================================================================================//
//  Methods.																																   //
//=============================================================================================================================================//

// Resizing the texture for when the window changes size.
void FrameBufferObject::resize(int width, int height)
{
	//// Bind MSAA FBO and delete textures.
	//GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_msaaFrameBufferID));
	//GLCall(glDeleteTextures(1, &m_msaaColorTextureID));
	//GLCall(glDeleteTextures(1, &m_msaaDepthStencilBufferID));
	//// Bind render FBO and delete textures.
	//GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_renderFrameBufferID));
	//GLCall(glDeleteTextures(1, &m_renderColorTextureID));
	// Recreate the attachments with the new size.
	createAttachments(width, height);
	// Save the dimenions.
	m_viewport[0] = width;
	m_viewport[1] = height;
}

// Bind the FBO.
void FrameBufferObject::bind()	  { GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_msaaFrameBufferID)); }
// Unbind the FBO.
void FrameBufferObject::unbind()  { GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0)); }
// Clear the FBO.
void FrameBufferObject::clear()	  { GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)); }

// Get access to the render texture ID.
// This renders the multisampled FBo the render FBO, which is not multisampled,
// but does have the processing effect.
unsigned int FrameBufferObject::getRenderTexture()
{
	// Resolve the MSAA and copy to the render FBO.
	GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_msaaFrameBufferID));
	GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_renderFrameBufferID));
	GLCall(glReadBuffer(GL_COLOR_ATTACHMENT0));
	GLCall(glDrawBuffer(GL_COLOR_ATTACHMENT0));
	GLCall(glBlitFramebuffer(0, 0, m_viewport[0], m_viewport[1], 0, 0, m_viewport[0], m_viewport[1], GL_COLOR_BUFFER_BIT, GL_LINEAR));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	return m_renderColorTextureID;
}	

// Return the ID of the entity at the pixel coordinates.
unsigned int FrameBufferObject::getEntityID(float pixelCoords[2]) 
{
	int entityID = -1; 
	// Resolve the MSAA and copy to the render FBO.
	GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_msaaFrameBufferID));
	GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_renderFrameBufferID));
	GLCall(glReadBuffer(GL_COLOR_ATTACHMENT1));
	GLCall(glDrawBuffer(GL_COLOR_ATTACHMENT1));
	GLCall(glBlitFramebuffer(0, 0, m_viewport[0], m_viewport[1], 0, 0, m_viewport[0], m_viewport[1], GL_COLOR_BUFFER_BIT, GL_NEAREST));
	// Read the pixel value.
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_renderFrameBufferID));
	GLCall(glReadBuffer(GL_COLOR_ATTACHMENT1));
	GLCall(glReadPixels((int)pixelCoords[0], (int)pixelCoords[1], 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &entityID));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	return entityID;
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//