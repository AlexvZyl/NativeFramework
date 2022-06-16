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
	LUMEN_RENDER_PASS();

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

	// Restore rendering state.
	destFBO.unbind();
}

void Renderer::blit(FrameBufferObject& sourceFBO, FrameBufferAttachmentSlot sourceSlot, FrameBufferObject& destFBO, FrameBufferAttachmentSlot destSlot, GLenum filter)
{
	// Get sizes.
	int width_s = sourceFBO.getSpecification().width;
	int height_s = sourceFBO.getSpecification().height;
	int width_d = destFBO.getSpecification().width;
	int height_d = destFBO.getSpecification().height;

	// Bind the buffers.
	GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, sourceFBO.getID()));
	GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destFBO.getID()));

	// Blit.
	GLCall(glReadBuffer((GLenum)sourceSlot));
	GLCall(glDrawBuffer((GLenum)destSlot));
	GLCall(glBlitFramebuffer(0, 0, width_s, height_s, 0, 0, width_d, height_d, GL_COLOR_BUFFER_BIT, filter));

	// Restore rendering state.
	destFBO.unbind();
}