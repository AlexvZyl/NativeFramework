#include "FrameBufferObjectGL.h"
#include "Utilities/Assert/Assert.h"
#include "OpenGL/ErrorHandlerGL.h"

FrameBufferObject::~FrameBufferObject() 
{
	if (m_isOnGPU) destroy();
}

void FrameBufferObject::create() 
{
	LUMEN_DEBUG_ASSERT(!m_isOnGPU, "Framebuffer already exists on the GPU.");

	GLCall(glCreateFramebuffers(1, &m_rendererID));
	m_isOnGPU = true;
	bind();
	createAttachments();
	unbind();
}

void FrameBufferObject::destroy() 
{
	LUMEN_DEBUG_ASSERT(m_isOnGPU, "Framebuffer is not on the GPU.");

	destroyAttachments();
	GLCall(glDeleteFramebuffers(1, &m_rendererID));
	m_isOnGPU = false;
}

void FrameBufferObject::bind() 
{
	LUMEN_DEBUG_ASSERT(m_isOnGPU, "Framebuffer is not on the GPU.");

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID));
	GLCall(glViewport(0, 0, m_specification.width, m_specification.height));
}

void FrameBufferObject::unbind()
{
	LUMEN_DEBUG_ASSERT(m_isOnGPU, "Framebuffer is not on the GPU.");

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FrameBufferObject::resize(int width, int height) 
{
	m_specification.width = width;
	m_specification.height = height;
	resize();
}

void FrameBufferObject::resize() 
{
	LUMEN_DEBUG_ASSERT(m_isOnGPU, "Framebuffer is not on the GPU.");

	resizeAttachments();
}

void FrameBufferObject::createAttachment(FrameBufferAttachment& attachment) 
{
	LUMEN_DEBUG_ASSERT(m_isOnGPU, "Framebuffer is not on the GPU.");

	// Generate.
	switch (attachment.type) 
	{
	case FrameBufferAttachmentType::TEXTURE_BUFFER:
		GLCall(glGenTextures(1, &attachment.rendererID));
		break;
	case FrameBufferAttachmentType::TEXTURE_STORAGE:
		GLCall(glGenTextures(1, &attachment.rendererID));
		break;
	case FrameBufferAttachmentType::RENDER_BUFFER:
		GLCall(glGenRenderbuffers(1, &attachment.rendererID));
		break;
	default:
		LUMEN_ASSERT(false, "Unknown attachment type.");
		break;
	}

	// Texture attachment data.
	if (attachment.type != FrameBufferAttachmentType::RENDER_BUFFER)
	{
		// MSAA.
		if (attachment.isMultiSample())
		{
			GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, attachment.rendererID));
			GLCall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, (int)attachment.samples, (GLenum)attachment.format, m_specification.width, m_specification.height, GL_FALSE));
			GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, (GLenum)attachment.slot, GL_TEXTURE_2D_MULTISAMPLE, attachment.rendererID, 0));
		}
		// Normal.
		else
		{
			GLCall(glBindTexture(GL_TEXTURE_2D, attachment.rendererID));
			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, (GLenum)attachment.internalFormat, m_specification.width, m_specification.height, 0, (GLenum)attachment.format, GL_UNSIGNED_BYTE, nullptr));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLenum)attachment.minFilter));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLenum)attachment.magFilter));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, (GLenum)attachment.wrapR));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLenum)attachment.wrapS));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLenum)attachment.wrapT));
			GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, (GLenum)attachment.slot, GL_TEXTURE_2D, attachment.rendererID, 0));
		}
	}

	// Render buffer data.
	else if (attachment.type == FrameBufferAttachmentType::RENDER_BUFFER)
	{
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, attachment.rendererID));
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER, (GLenum)attachment.internalFormat, m_specification.width, m_specification.height));
		GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, (GLenum)attachment.slot, GL_RENDERBUFFER, attachment.rendererID));
	}

	// Error.
	else LUMEN_ASSERT(false, "Unknown attachment type.");
}

void FrameBufferObject::destroyAttachment(const FrameBufferAttachment& attachment)
{
	LUMEN_DEBUG_ASSERT(m_isOnGPU, "Framebuffer is not on the GPU.");

	switch (attachment.type)
	{
	case FrameBufferAttachmentType::TEXTURE_BUFFER:
		GLCall(glDeleteTextures(1, &attachment.rendererID));
		break;

	case FrameBufferAttachmentType::TEXTURE_STORAGE:
		GLCall(glDeleteTextures(1, &attachment.rendererID));
		break;

	case FrameBufferAttachmentType::RENDER_BUFFER:
		GLCall(glDeleteRenderbuffers(1, &attachment.rendererID));
		break;

	default:
		LUMEN_ASSERT(false, "Unknown attachment type.");
		break;
	}
}

void FrameBufferObject::clearAttachment(const FrameBufferAttachment& attachment, int value) 
{
	LUMEN_DEBUG_ASSERT(m_isOnGPU, "Framebuffer is not on the GPU.");

	switch (attachment.type)
	{
	case FrameBufferAttachmentType::TEXTURE_BUFFER:
		GLCall(glClearTexImage(attachment.rendererID, 0, (GLenum)attachment.format, GL_INT, &value));
		break;

	case FrameBufferAttachmentType::TEXTURE_STORAGE:
		GLCall(glClearTexImage(attachment.rendererID, 0, (GLenum)attachment.format, GL_INT, &value));
		break;

	case FrameBufferAttachmentType::RENDER_BUFFER:
		// ? 
		break;

	default:
		LUMEN_ASSERT(false, "Unknown attachment type.");
		break;
	}
}

void FrameBufferObject::resizeAttachment(const FrameBufferAttachment& attachment) 
{
	LUMEN_DEBUG_ASSERT(m_isOnGPU, "Framebuffer is not on the GPU.");

	// Texture.
	if (attachment.type != FrameBufferAttachmentType::RENDER_BUFFER)
	{
		// MSAA.
		if (attachment.isMultiSample())
		{
			GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, attachment.rendererID));
			GLCall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, (int)attachment.samples, (GLenum)attachment.format, m_specification.width, m_specification.height, GL_FALSE));
		}
		// Normal.
		else
		{
			GLCall(glBindTexture(GL_TEXTURE_2D, attachment.rendererID));
			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, (GLenum)attachment.internalFormat, m_specification.width, m_specification.height, 0, (GLenum)attachment.format, GL_UNSIGNED_BYTE, nullptr));
		}
	}

	// Render buffer.
	else if (attachment.type == FrameBufferAttachmentType::RENDER_BUFFER)
	{
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, attachment.rendererID));
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER, (GLenum)attachment.internalFormat, m_specification.width, m_specification.height));
	}

	// Error.
	else LUMEN_ASSERT(false, "Unknown attachment type.");
}

void FrameBufferObject::bindDrawBuffers() 
{
	LUMEN_DEBUG_ASSERT(m_isOnGPU, "Framebuffer is not on the GPU.");

	GLCall(glNamedFramebufferDrawBuffers(m_rendererID, m_drawBuffers.size(), (GLenum*)m_drawBuffers.data()));
}

void FrameBufferObject::bindReadBuffer() 
{
	LUMEN_DEBUG_ASSERT(m_isOnGPU, "Framebuffer is not on the GPU.");

	GLCall(glNamedFramebufferReadBuffer(m_rendererID, (GLenum)m_readBuffer));
}

void FrameBufferObject::addAttachment(const FrameBufferAttachment& attachment)
{
	m_attachments[attachment.slot] = attachment;
	attachmentsChanged();
}

void FrameBufferObject::removeAttachment(FrameBufferAttachmentSlot slot)
{
	m_attachments.erase(slot);
	attachmentsChanged();
}

void FrameBufferObject::destroyAttachments() 
{
	for (auto& [slot, attachment] : m_attachments) destroyAttachment(attachment);
}

void FrameBufferObject::destroyAttachment(FrameBufferAttachmentSlot slot)
{
	destroyAttachment(m_attachments[slot]);
}

void FrameBufferObject::clearAttachments(int value)
{
	for (auto& [slot, attachment] : m_attachments) clearAttachment(attachment, value);
}

void FrameBufferObject::clearAttachment(FrameBufferAttachmentSlot slot, int value) 
{
	clearAttachment(m_attachments[slot], value);
}

void FrameBufferObject::resizeAttachments() 
{
	for (auto& [slot, attachment] : m_attachments) resizeAttachment(attachment);
}

void FrameBufferObject::resizeAttachment(FrameBufferAttachmentSlot slot) 
{
	resizeAttachment(m_attachments[slot]);
}

void FrameBufferObject::createAttachments()
{
	for (auto& [slot, attachment] : m_attachments) createAttachment(attachment);
}

void FrameBufferObject::createAttachment(FrameBufferAttachmentSlot slot) 
{
	createAttachment(m_attachments[slot]);
}