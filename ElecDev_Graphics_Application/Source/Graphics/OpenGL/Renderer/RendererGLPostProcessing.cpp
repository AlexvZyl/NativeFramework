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
	int samples = (int)sourceFBO.getAttachment(sourceSlot).samples;
	Shader& shader = *Renderer::s_shaders["MSAA"].get();
	shader.bind();
	int textureSize[2] = { sourceFBO.getSpecification().width, sourceFBO.getSpecification().height };
	shader.setIntArray("textureSize", textureSize, 2);
	shader.setInt("msaaSamples", samples);
	shader.setSamplerMSAA("msaaTexture", (int*)&sourceFBO.getAttachment(sourceSlot).rendererID, 1);

	// Setup rendering state.
	destFBO.bind();
	GLCall(glDrawBuffer((GLenum)destSlot));
	Renderer::drawBufferIndexed(*s_unitQuad.get());

	// Restore rendering state.
	destFBO.bindDrawBuffers();
	destFBO.unbind();
}