//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

// Class include.
#include <glad/glad.h>
#include "Resources/ResourceHandler.h"
#include "External/Misc/ConsoleColor.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "OpenGL/Buffers/FrameBufferObjectGL.h"
#include "OpenGL/Buffers/VertexArrayObjectGL.h"
#include "OpenGL/Primitives/Vertex.h"
#include "OpenGL/ErrorHandlerGL.h"
#include "OpenGL/ShaderGL.h"
#include "Utilities/Logger/Logger.h"

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

FrameBufferObject::FrameBufferObject(int width, int height) 
{
	// Create the attachments and the FBOs.
	createResources(width, height);
}

void FrameBufferObject::initAttachments(int width, int height)
{
	// Bind render FBO.
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferID));
	// Color.
	GLCall(glBindTexture(GL_TEXTURE_2D, m_colorTextureID));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTextureID, 0));
	// Entity ID.
	GLCall(glBindTexture(GL_TEXTURE_2D, m_entityIDTextureID));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, NULL));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_entityIDTextureID, 0));
	// Outline.
	GLCall(glBindTexture(GL_TEXTURE_2D, m_outlineColorTextureID));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_outlineColorTextureID, 0));
	// Depth/Stencil.
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilID));
	GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthStencilID));
	
	// Setup.
	setDrawBuffers();
	
	// Errors.
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LUMEN_LOG_ERROR("FBO craetion failed.", "Renderer");
	}

	// Cleanup.
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

// Destructor.
FrameBufferObject::~FrameBufferObject()
{
	deleteResources();
}

void FrameBufferObject::deleteResources() 
{
	// Do not delete if already deleted.
	if (m_resourcesDeleted) return;
	m_resourcesDeleted = true;

	// Delete attachments.
	GLCall(glDeleteRenderbuffers(1, &m_depthStencilID));
	GLCall(glDeleteTextures(1, &m_colorTextureID));
	GLCall(glDeleteTextures(1, &m_entityIDTextureID));
	GLCall(glDeleteTextures(1, &m_outlineColorTextureID));
	// Delete FBOs.
	GLCall(glDeleteFramebuffers(1, &m_frameBufferID));
}

void FrameBufferObject::createResources(int width, int height)
{
	// Do not create a FBO with size or width of zero.
	if (!width || !height)
	{
		LUMEN_LOG_WARN("Tried to create a FBO with width or height of zero.", "Renderer");
		return;
	}

	// Do not recreate if already exist.
	if (!m_resourcesDeleted) return;
	m_resourcesDeleted = false;

	// Generate render FBO.
	GLCall(glGenFramebuffers(1, &m_frameBufferID));
	GLCall(glGenRenderbuffers(1, &m_depthStencilID));
	GLCall(glGenTextures(1, &m_colorTextureID));
	GLCall(glGenTextures(1, &m_outlineColorTextureID));
	GLCall(glGenTextures(1, &m_entityIDTextureID));

	// Check for generation error.
	if (!glCheckFramebufferStatus(GL_FRAMEBUFFER))
	{
		LUMEN_LOG_ERROR("FBO could not be generated.", "Renderer");
	}

	// Create attachments for the FBO's.
	initAttachments(width, height);

	// Save the dimenions.
	m_viewport[0] = width;
	m_viewport[1] = height;
}

//=============================================================================================================================================//
//  Methods.																																   //
//=============================================================================================================================================//

void FrameBufferObject::resize(int width, int height)
{
	if (m_resourcesDeleted) return;

	// Depth/Stencil.
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilID));
	GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
	// Color.
	GLCall(glBindTexture(GL_TEXTURE_2D, m_colorTextureID));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
	// Entity ID.
	GLCall(glBindTexture(GL_TEXTURE_2D, m_entityIDTextureID));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, NULL));
	// Outline.
	GLCall(glBindTexture(GL_TEXTURE_2D, m_outlineColorTextureID));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));

	// Error.
	if (!glCheckFramebufferStatus(GL_FRAMEBUFFER))
	{
		LUMEN_LOG_ERROR("FBO could not be resized.", "Renderer");
	}

	// Save the dimenions.
	m_viewport[0] = width;
	m_viewport[1] = height;
}

unsigned FrameBufferObject::getRenderTexture() 
{ 
	return m_colorTextureID; 
}

void FrameBufferObject::bind() 
{ 
	if (m_resourcesDeleted) return;

	// Bind the FBO.
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferID));
}
												   
void FrameBufferObject::unbind() 
{ 
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0)); 
}

void FrameBufferObject::clear()	  
{ 
	if (m_resourcesDeleted) return;

	GLCall(glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
	GLCall(glClearTexImage(m_colorTextureID, 0, GL_RGBA, GL_FLOAT, 0));
	GLCall(glClearTexImage(m_outlineColorTextureID, 0, GL_RGBA, GL_FLOAT, 0));
}

unsigned int FrameBufferObject::getEntityID(const glm::vec2& pixelCoords) 
{
	if (m_resourcesDeleted) return -1;

	int entityID = -1; 
	// Read the pixel value.
	bind();
	GLCall(glReadBuffer(GL_COLOR_ATTACHMENT1));
	GLCall(glReadPixels((int)pixelCoords.x, (int)pixelCoords.y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &entityID));
	return entityID;
}

void FrameBufferObject::setDrawBuffers() 
{
	GLenum drawBuffers[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	GLCall(glNamedFramebufferDrawBuffers(m_frameBufferID, 3, drawBuffers));
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
