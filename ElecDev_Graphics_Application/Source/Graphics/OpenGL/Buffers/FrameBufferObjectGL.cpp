//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

// Class include.
#include <glad/glad.h>
#include "Resources/ResourceHandler.h"
#include "External/Misc/ConsoleColor.h"
#include "OpenGL/Buffers/FrameBufferObjectGL.h"
#include "OpenGL/ErrorHandlerGL.h"
#include "OpenGL/Buffers/VertexArrayObjectGL.h"
#include "OpenGL/Entities/Vertex.h"
#include "OpenGL/ShaderGL.h"

//=============================================================================================================================================//
//  Static inits.																															   //
//=============================================================================================================================================//

std::unique_ptr<Shader> FrameBufferObject::m_shader;

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

// Default.
FrameBufferObject::FrameBufferObject() {};

// Constructor.
FrameBufferObject::FrameBufferObject(int width, int height, int MSAA) 
	: m_MSAA(MSAA)
{
	// Do not create a FBO with size or width of zero.
	if (!width || !height)
	{
		std::cout << yellow << "[OPENGL] [WARN]: " << white << "Tried to create a FBO with width or height of zero.";
		return;
	}

	// Generate MSAA FBO.
	GLCall(glGenFramebuffers(1, &m_msaaFrameBufferID));
	GLCall(glGenTextures(1, &m_msaaColorTextureID));
	GLCall(glGenTextures(1, &m_msaaEntityIDTextureID));
	GLCall(glGenRenderbuffers(1, &m_msaaDepthStencilBufferID));
	// Check for generation error.
	if (!glCheckFramebufferStatus(GL_FRAMEBUFFER))
	{ std::cout << red << "\n\n[OPENGL] [ERROR] :" << white << " MSAA FBO could not be generated.\n"; }

	// Generate render FBO.
	GLCall(glGenFramebuffers(1, &m_renderFrameBufferID));
	GLCall(glGenTextures(1, &m_renderColorTextureID));
	GLCall(glGenTextures(1, &m_renderEntityIDTextureID));
	// Check for generation error.
	if (!glCheckFramebufferStatus(GL_FRAMEBUFFER))
	{ std::cout << red << "\n\n[OPENGL] [ERROR] :" << white << " Render FBO could not be generated.\n"; }

	// Post processing.

	// Create attachments for the FBO's.
	createAttachments(width, height);

	// Save the dimenions.
	m_viewport[0] = width;
	m_viewport[1] = height;

	// ----------------------- //
	//  R E N D E R   Q U A D  //
	// ----------------------- //

	// Create the quad.
	m_renderVAO = std::make_unique<VertexArrayObject<VertexDataTextured>>(GL_TRIANGLES);
	std::vector<std::unique_ptr<VertexDataTextured>> vertices;
	vertices.reserve(4);
	vertices.emplace_back(std::make_unique<VertexDataTextured>(-1.f, -1.f,  0.f,   1.f, 0.f, 0.f, 1.f,   0.f, 0.f,   2.f,  1));
	vertices.emplace_back(std::make_unique<VertexDataTextured>(-1.f,  1.f,  0.f,   0.f, 1.f, 0.f, 1.f,   0.f, 1.f,   2.f,  1));
	vertices.emplace_back(std::make_unique<VertexDataTextured>( 1.f,  1.f,  0.f,   0.f, 0.f, 1.f, 1.f,   1.f, 1.f,   2.f,  1));
	vertices.emplace_back(std::make_unique<VertexDataTextured>( 1.f, -1.f,  0.f,   1.f, 0.f, 1.f, 1.f,   1.f, 0.f,   2.f,  1));
	std::vector<unsigned> indices = { 0,1,2, 2,3,0 };
	m_renderVAO->appendVertexData(vertices, indices);
	unsigned texture = loadBitmapToGL(loadImageFromResource(CIRCUIT_TREE_PNG));
	int msaaTextureID = 2;	// Reserved for FBO redering.
	m_shader->bind();
	m_shader->setInt("msaaSamples", m_MSAA);
	m_shader->setSamplerMSAA("msaaTexture", &msaaTextureID, 1);
	int viewport[2] = { (int)m_viewport.x, (int)m_viewport.y };
	m_shader->setIntArray("textureSize", viewport, 2);
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
	GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_msaaColorTextureID));
	GLCall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_MSAA, GL_RGBA8, width, height, GL_TRUE));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_msaaColorTextureID, 0));
	// Generate and attach MSAA entityID buffer.
	GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_msaaEntityIDTextureID));
	GLCall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_MSAA, GL_R32UI, width, height, GL_TRUE));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D_MULTISAMPLE, m_msaaEntityIDTextureID, 0));
	// Create depth/stencil MSAA buffer texture.
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
	GLCall(glBindTexture(GL_TEXTURE_2D, m_renderColorTextureID));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_renderColorTextureID, 0));
	// Generate Entity ID texture.
	GLCall(glBindTexture(GL_TEXTURE_2D, m_renderEntityIDTextureID));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, NULL));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_renderEntityIDTextureID, 0));

	// ------------------------------------- //
	//  R E N D E R   A T T A C H M E N T S  //
	// ------------------------------------- //

	// Check for completion errror.
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << red << "\n\n[OPENGL] [ERROR] :" << white << " Render FBO is not complete.\n" << std::endl;

	// Enable draw buffers.
	GLenum drawBuffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	GLCall(glNamedFramebufferDrawBuffers(m_msaaFrameBufferID, 2, drawBuffers));
	GLCall(glNamedFramebufferDrawBuffers(m_renderFrameBufferID, 2, drawBuffers));

	// --------------- //
	//  C L E A N U P  //
	// --------------- //

	// Unbind.
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

// Destructor.
FrameBufferObject::~FrameBufferObject()
{
	// Delete the textures.
	GLCall(glDeleteTextures(1, &m_msaaColorTextureID));
	GLCall(glDeleteRenderbuffers(1, &m_msaaDepthStencilBufferID));
	GLCall(glDeleteTextures(1, &m_msaaEntityIDTextureID));
	GLCall(glDeleteTextures(1, &m_renderColorTextureID));
	GLCall(glDeleteTextures(1, &m_renderEntityIDTextureID));
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
	// This is not the best wau to resize an FBO.
	// 

	// Recreate the attachments with the new size.
	createAttachments(width, height);
	// Save the dimenions.
	m_viewport[0] = width;
	m_viewport[1] = height;
	// Change texture size variable in  shader.
	m_shader->bind();
	int viewport[2] = { (int)m_viewport.x, (int)m_viewport.y };
	m_shader->setIntArray("textureSize", viewport, 2);
}

unsigned FrameBufferObject::getRenderTexture() 
{ 
	return m_renderColorTextureID; 
}

void FrameBufferObject::bind() 
{ 
	// Bind the FBO.
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_msaaFrameBufferID));
}

void FrameBufferObject::bindRender() 
{ 
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_renderFrameBufferID)); 
}
												   
void FrameBufferObject::unbind() 
{ 
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0)); 
}

void FrameBufferObject::clear()	  
{ 
	GLCall(glClear(GL_DEPTH_BUFFER_BIT));									// Clear depth buffer.
	GLCall(glClearTexImage(m_msaaColorTextureID, 0, GL_RGBA, GL_FLOAT, 0));	// Clear color attachment.
}

void FrameBufferObject::clearRender()
{
	GLCall(glClear(GL_DEPTH_BUFFER_BIT));									  // Clear depth buffer.
	GLCall(glClearTexImage(m_renderColorTextureID, 0, GL_RGBA, GL_FLOAT, 0)); // Clear color attachment.
}

unsigned int FrameBufferObject::getEntityID(glm::vec2& pixelCoords) 
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
	GLCall(glReadPixels((int)pixelCoords.x, (int)pixelCoords.y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &entityID));
	// Enable draw buffers.
	GLenum drawBuffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	GLCall(glNamedFramebufferDrawBuffers(m_msaaFrameBufferID, 2, drawBuffers));
	GLCall(glNamedFramebufferDrawBuffers(m_renderFrameBufferID, 2, drawBuffers));
	return entityID;
}

void FrameBufferObject::blitFromMSAA()
{
	// Resolve the MSAA and copy to the render FBO.
	GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_msaaFrameBufferID));
	GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_renderFrameBufferID));
	GLCall(glReadBuffer(GL_COLOR_ATTACHMENT0));
	GLCall(glDrawBuffer(GL_COLOR_ATTACHMENT0));
	GLCall(glBlitFramebuffer(0, 0, m_viewport[0], m_viewport[1], 0, 0, m_viewport[0], m_viewport[1], GL_COLOR_BUFFER_BIT, GL_LINEAR));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	// Enable draw buffers.
	GLenum drawBuffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	GLCall(glNamedFramebufferDrawBuffers(m_msaaFrameBufferID, 2, drawBuffers));
	GLCall(glNamedFramebufferDrawBuffers(m_renderFrameBufferID, 2, drawBuffers));
}	

void FrameBufferObject::renderFromMSAA() 
{
	GLCall(glViewport(0,0, m_viewport[0], m_viewport[1]));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_renderFrameBufferID));		  
	GLCall(glClear(GL_DEPTH_BUFFER_BIT));									  
	GLCall(glClearTexImage(m_renderColorTextureID, 0, GL_RGBA, GL_FLOAT, 0)); 
	GLCall(glActiveTexture(GL_TEXTURE2));									  
    GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_msaaColorTextureID));	  
	m_shader->bind();		
	// Update the data in the shader.
	int viewport[2] = { (int)m_viewport.x, (int)m_viewport.y };
	m_shader->setIntArray("textureSize", viewport, 2);
	m_renderVAO->render();													  
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));							  
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
