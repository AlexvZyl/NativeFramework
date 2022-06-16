//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "OpenGL/Renderer/RendererGL.h"
#include "OpenGL/ErrorHandlerGL.h"
#include "OpenGL/ShaderGL.h" 
#include "OpenGL/Buffers/GraphicsPrimitivesBuffersGL.h"
#include "OpenGL/Buffers/FrameBufferObjectGL.h"
#include "Application/Application.h"
#include "Lumen/Lumen.h"
#include "OpenGL/Primitives/Vertex.h"

//==============================================================================================================================================//
//  Buffers.																																	//
//==============================================================================================================================================//

void Renderer::drawBufferIndexed(IGraphicsPrimitivesBuffer& gpb)
{
	if (!gpb.onDrawCall()) return;

	LUMEN_DRAW_CALL();

	VertexArrayObject& vao = gpb.getVAO();
	vao.bind();
	vao.getIBO().bind();
	GLCall(glDrawElements(vao.getType(), gpb.getIndexCount(), GL_UNSIGNED_INT, 0));
}

void Renderer::drawBufferIndexedForcePrimitive(IGraphicsPrimitivesBuffer& gpb, unsigned primitive)
{
	if (!gpb.onDrawCall()) return;

	LUMEN_DRAW_CALL();

	VertexArrayObject& vao = gpb.getVAO();
	vao.bind();
	vao.getIBO().bind();
	GLCall(glDrawElements(primitive, gpb.getIndexCount(), GL_UNSIGNED_INT, 0));
}

//==============================================================================================================================================//
//  Textures.																																	//
//==============================================================================================================================================//

void Renderer::drawTextureOverFBOAttachment(FrameBufferObject& FBO, FrameBufferAttachmentSlot slot, unsigned texture, Shader* shader)
{
	FrameBufferAttachment& attachment = FBO.getAttachment(slot);
	shader->bind();
	GLCall(glActiveTexture(GL_TEXTURE0));
	if (attachment.isMultiSample()) { GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture)); }
	else							{ GLCall(glBindTexture(GL_TEXTURE_2D, texture)); }
	GLCall(glNamedFramebufferDrawBuffers(FBO.getID(), 1, (GLenum*)&slot));
	Renderer::drawBufferIndexed(*s_unitQuad.get());
	if (attachment.isMultiSample()) { GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0)); }
	else							{ GLCall(glBindTexture(GL_TEXTURE_2D, texture)); }
	FBO.bindDrawBuffers();
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//