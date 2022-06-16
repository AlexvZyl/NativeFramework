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

void Renderer::drawTextureOverFBOAttachment(FrameBufferObject* FBO, unsigned texture, unsigned attachment, Shader* shader) 
{
	shader->bind();
	GLCall(glActiveTexture(GL_TEXTURE0));
	//GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture));
	GLCall(glBindTexture(GL_TEXTURE_2D, texture));
	GLCall(glNamedFramebufferDrawBuffers(FBO->getID(), 1, &attachment));
	Renderer::drawBufferIndexed(*s_unitQuad.get());
	//GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	FBO->bindDrawBuffers();
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//