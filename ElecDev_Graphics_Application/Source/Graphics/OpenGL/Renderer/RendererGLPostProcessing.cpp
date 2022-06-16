#include "OpenGL/Buffers/GraphicsPrimitivesBuffersGL.h"
#include "OpenGL/Buffers/FrameBufferObjectGL.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "OpenGL/ErrorHandlerGL.h"
#include "OpenGL/ShaderGL.h"
#include "Lumen/Lumen.h"
#include "Application/Application.h"

#include "glad/glad.h"

void Renderer::resolveMSAA(FrameBufferObject& sourceFBO, FrameBufferAttachmentSlot sourceSlot, FrameBufferObject& destFBO, FrameBufferAttachmentSlot destSlot) 
{
	// Setup shader.
	int sampler = 0;
	int samples = (int)sourceFBO.getAttachment(sourceSlot).samples;
	Shader& shader = *Renderer::s_shaders["MSAA"].get();
	int textureSize[2] = { sourceFBO.getSpecification().width, sourceFBO.getSpecification().height };
	shader.bind();
	shader.setIntArray("textureSize", textureSize, 2);
	shader.setSamplerMSAA("msaaTexture", &sampler, 1);
	shader.setInt("msaaSamples", samples);

	// Setup rendering state.
	destFBO.bind();
	GLCall(glDrawBuffer((GLenum)destSlot));
	GLCall(glActiveTexture(GL_TEXTURE0));
	GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, sourceFBO.getAttachment(sourceSlot).rendererID));

	// Render.
	Renderer::drawBufferIndexed(*s_unitQuad.get());
}

void Renderer::blit(FrameBufferObject& sourceFBO, FrameBufferAttachmentSlot sourceSlot, FrameBufferObject& destFBO, FrameBufferAttachmentSlot destSlot, GLenum filter)
{
	GLCall(glNamedFramebufferReadBuffer(sourceFBO.getID(), (GLenum)sourceSlot));
	GLCall(glNamedFramebufferDrawBuffer(destFBO.getID(), (GLenum)destSlot));
	GLCall(glBlitNamedFramebuffer(sourceFBO.getID(), destFBO.getID(), 
								  0, 0, sourceFBO.getSpecification().width, sourceFBO.getSpecification().height, 
								  0, 0, destFBO.getSpecification().width, destFBO.getSpecification().height, 
								  GL_COLOR_BUFFER_BIT, filter));
}