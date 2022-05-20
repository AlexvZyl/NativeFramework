//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "OpenGL/Buffers/VertexArrayObjectGL.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "OpenGL/ErrorHandlerGL.h"
#include "OpenGL/ShaderGL.h" 
#include "OpenGL/Buffers/FrameBufferObjectGL.h"
#include "Application/Application.h"
#include "Lumen/Lumen.h"

//==============================================================================================================================================//
//  Buffers.																																	//
//==============================================================================================================================================//

void Renderer::drawBufferIndexed(VertexArrayObjectPtr* vao)
{
	if (!vao->onDrawCall()) return;

	LUMEN_DRAW_CALL();

	GLCall(glBindVertexArray(vao->m_VAOID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao->m_IBOID));
	GLCall(glDrawElements(vao->m_bufferType, vao->m_indexCount, GL_UNSIGNED_INT, 0));
}

void Renderer::drawBufferIndexedForcePrimitive(VertexArrayObjectPtr* vao, unsigned primitive)
{
	if (!vao->onDrawCall()) return;

	LUMEN_DRAW_CALL();

	GLCall(glBindVertexArray(vao->m_VAOID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao->m_IBOID));
	GLCall(glDrawElements(primitive, vao->m_indexCount, GL_UNSIGNED_INT, 0));
}

//==============================================================================================================================================//
//  Textures.																																	//
//==============================================================================================================================================//

void Renderer::drawTextureOverFBOAttachment(FrameBufferObject* FBO, unsigned texture, unsigned attachment, Shader* shader) 
{
	shader->bind();
	GLCall(glActiveTexture(GL_TEXTURE0));
	GLCall(glBindTexture(GL_TEXTURE_2D, texture));
	GLenum drawBuffers[1] = { attachment };
	GLCall(glNamedFramebufferDrawBuffers(FBO->m_frameBufferID, 1, drawBuffers));
	Renderer::drawBufferIndexed(s_unitQuad.get());
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	FBO->setDrawBuffers();
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//