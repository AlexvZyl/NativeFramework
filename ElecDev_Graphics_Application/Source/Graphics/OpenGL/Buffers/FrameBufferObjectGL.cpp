#include "FrameBufferObjectGL.h"
#include "Utilities/Assert/Assert.h"
#include "OpenGL/ErrorHandlerGL.h"
#include "Utilities/Logger/Logger.h"
#include "OpenGL/Renderer/RendererGL.h"

FrameBufferObject::~FrameBufferObject() 
{
	if (m_isOnGPU) destroy();
}

void FrameBufferObject::create() 
{
	LUMEN_DEBUG_ASSERT(!m_isOnGPU, "Framebuffer already exists on the GPU.");

	// Create FBO.
	GLCall(glCreateFramebuffers(1, &m_rendererID));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID));
	m_isOnGPU = true;

	// Create attachments.
	createAttachments();

	// Error checking.
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		LUMEN_LOG_ERROR("Framebuffer not complete.", "Renderer");
	
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FrameBufferObject::create(int width, int height)
{
	setSize(width, height);
	create();
}

void FrameBufferObject::clear() 
{
	LUMEN_DISABLED_FUNCTION();

	LUMEN_DEBUG_ASSERT(m_isOnGPU, "Framebuffer is not on the GPU.");
	
	GLCall(glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT));
	clearAttachments();
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
	LUMEN_DEBUG_ASSERT(m_isOnGPU, "Framebuffer is not on the GPU.");

	setSize(width, height);
	resizeAttachments();

	// Error checking.
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		LUMEN_LOG_ERROR("Framebuffer resize failed.", "Renderer");
}

void FrameBufferObject::createAttachment(FrameBufferAttachment& attachment) 
{
	LUMEN_DEBUG_ASSERT(m_isOnGPU, "Framebuffer is not on the GPU.");
	LUMEN_DEBUG_ASSERT(!attachment.created, "Attachment has already been created.");

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
		GLCall(glCreateRenderbuffers(1, &attachment.rendererID));
		break;
	default:
		LUMEN_ASSERT(false, "Unknown attachment type.");
		break;
	}

	// Texture buffer.
	if (attachment.type == FrameBufferAttachmentType::TEXTURE_BUFFER)
	{
		// MSAA.
		if (attachment.isMultiSample())
		{
			GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, attachment.rendererID));
			GLCall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, (int)attachment.samples, (GLenum)attachment.internalFormat, m_specification.width, m_specification.height, GL_TRUE));
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

	// Texture storage.
	else if (attachment.type == FrameBufferAttachmentType::TEXTURE_STORAGE) 
	{
		// MSAA.
		if (attachment.isMultiSample())
		{
			GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, attachment.rendererID));
			GLCall(glTexStorage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, (int)attachment.samples, (GLenum)attachment.internalFormat, m_specification.width, m_specification.height, GL_TRUE));
			GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, (GLenum)attachment.slot, GL_TEXTURE_2D_MULTISAMPLE, attachment.rendererID, 0));
		}
		// Normal.
		else
		{
			GLCall(glBindTexture(GL_TEXTURE_2D, attachment.rendererID));
			GLCall(glTexStorage2D(GL_TEXTURE_2D, 1, (GLenum)attachment.internalFormat, m_specification.width, m_specification.height));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLenum)attachment.minFilter));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLenum)attachment.magFilter));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, (GLenum)attachment.wrapR));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLenum)attachment.wrapS));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLenum)attachment.wrapT));
			GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, (GLenum)attachment.slot, GL_TEXTURE_2D, attachment.rendererID, 0));
		}
	}

	// Render buffer.glTexImage2DMultisample
	else if (attachment.type == FrameBufferAttachmentType::RENDER_BUFFER)
	{
		// MSAA.
		if (attachment.isMultiSample())
		{
			GLCall(glNamedRenderbufferStorageMultisample(attachment.rendererID, (int)attachment.samples, (GLenum)attachment.internalFormat, m_specification.width, m_specification.height));
		}
		// Normal.
		else 
		{
			GLCall(glNamedRenderbufferStorage(attachment.rendererID, (GLenum)attachment.internalFormat, m_specification.width, m_specification.height));
		}
		GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, (GLenum)attachment.slot, GL_RENDERBUFFER, attachment.rendererID));
	}

	// Error.
	else LUMEN_ASSERT(false, "Unknown attachment type.");

	// Error checking.
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
	{
		std::string msg = "Framebuffer attachment creation failed: " + attachment.slotString();
		LUMEN_LOG_ERROR(msg, "Renderer");
	}

	attachment.created = true;
}


void FrameBufferObject::resizeAttachment(const FrameBufferAttachment& attachment)
{
	LUMEN_DEBUG_ASSERT(m_isOnGPU, "Framebuffer is not on the GPU.");
	LUMEN_DEBUG_ASSERT(attachment.created, "Attachment has not been created.");

	// Texture buffer.
	if (attachment.type == FrameBufferAttachmentType::TEXTURE_BUFFER)
	{
		// MSAA.
		if (attachment.isMultiSample())
		{
			GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, attachment.rendererID));
			GLCall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, (int)attachment.samples, (GLenum)attachment.internalFormat, m_specification.width, m_specification.height, GL_TRUE));
		}
		// Normal.
		else
		{
			GLCall(glBindTexture(GL_TEXTURE_2D, attachment.rendererID));
			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, (GLenum)attachment.internalFormat, m_specification.width, m_specification.height, 0, (GLenum)attachment.format, GL_UNSIGNED_BYTE, nullptr));
		}
	}

	// Texture STORAGE.
	else if (attachment.type == FrameBufferAttachmentType::TEXTURE_STORAGE)
	{
		// MSAA.
		if (attachment.isMultiSample())
		{
			GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, attachment.rendererID));
			GLCall(glTexStorage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, (int)attachment.samples, (GLenum)attachment.internalFormat, m_specification.width, m_specification.height, GL_TRUE));
		}
		// Normal.
		else
		{
			GLCall(glBindTexture(GL_TEXTURE_2D, attachment.rendererID));
			GLCall(glTexStorage2D(GL_TEXTURE_2D, 1, (GLenum)attachment.internalFormat, m_specification.width, m_specification.height));
		}
	}

	// Render buffer.
	else if (attachment.type == FrameBufferAttachmentType::RENDER_BUFFER)
	{
		// MSAA.
		if (attachment.isMultiSample())
		{
			GLCall(glNamedRenderbufferStorageMultisample(attachment.rendererID, (int)attachment.samples, (GLenum)attachment.internalFormat, m_specification.width, m_specification.height));
		}
		// Normal.
		else
		{
			GLCall(glNamedRenderbufferStorage(attachment.rendererID, (GLenum)attachment.internalFormat, m_specification.width, m_specification.height));
		}
	}

	// Error.
	else LUMEN_ASSERT(false, "Unknown attachment type.");

	// Error checking.
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
	{
		std::string msg = "Framebuffer attachment resizing failed: " + attachment.slotString();
		LUMEN_LOG_ERROR(msg, "Renderer");
	}
}


void FrameBufferObject::destroyAttachment(FrameBufferAttachment& attachment)
{
	LUMEN_DEBUG_ASSERT(m_isOnGPU, "Framebuffer is not on the GPU.");
	LUMEN_DEBUG_ASSERT(attachment.created, "Attachment has not been created.");

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

	attachment.created = false;
}

void FrameBufferObject::clearAttachment(const FrameBufferAttachment& attachment) 
{
	LUMEN_DISABLED_FUNCTION();

	LUMEN_DEBUG_ASSERT(m_isOnGPU, "Framebuffer is not on the GPU.");
	LUMEN_DEBUG_ASSERT(attachment.created, "Attachment has not been created.");

	// Unsure why this will not work.
	if (attachment.internalFormat == FrameBufferTextureFormat::R32_UI) return;

	switch (attachment.type)
	{
	case FrameBufferAttachmentType::TEXTURE_BUFFER:
		GLCall(glClearTexImage(attachment.rendererID, 0, (GLenum)attachment.format, attachment.dataType(), 0));
		break;

	case FrameBufferAttachmentType::TEXTURE_STORAGE:
		GLCall(glClearTexImage(attachment.rendererID, 0, (GLenum)attachment.format, attachment.dataType(), 0));
		break;

	case FrameBufferAttachmentType::RENDER_BUFFER:
		// ? 
		break;

	default:
		LUMEN_ASSERT(false, "Unknown attachment type.");
		break;
	}
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

int FrameBufferObject::readPixel(FrameBufferAttachmentSlot slot, int x, int y) 
{
	LUMEN_DEBUG_ASSERT(m_isOnGPU, "Framebuffer is not on the GPU.");
	LUMEN_DEBUG_ASSERT(m_attachments[slot].created, "Attachment has not been created.");
	LUMEN_DEBUG_ASSERT(x >= 0 && x < m_specification.width, "Trying to read outside of attachment width.");
	LUMEN_DEBUG_ASSERT(y >= 0 && y < m_specification.height, "Trying to read outside of attachment height.");

	GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_rendererID));
	GLCall(glReadBuffer((GLenum)slot));
	int result;
	GLCall(glReadPixels(x, y, 1, 1, (GLenum)m_attachments[slot].format, m_attachments[slot].dataType(), &result));
	GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, 0));
	return result;
}

void FrameBufferObject::addAttachment(const FrameBufferAttachment& attachment)
{
	m_attachments[attachment.slot] = attachment;
	attachmentsChanged();
}

void FrameBufferObject::removeAttachment(FrameBufferAttachmentSlot slot)
{
	if (m_attachments[slot].created) destroyAttachment(slot);
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

void FrameBufferObject::clearAttachments()
{
	for (auto& [slot, attachment] : m_attachments) clearAttachment(attachment);
}

void FrameBufferObject::clearAttachment(FrameBufferAttachmentSlot slot) 
{
	clearAttachment(m_attachments[slot]);
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
	m_attachmentsChanged = false;
}

void FrameBufferObject::createAttachment(FrameBufferAttachmentSlot slot) 
{
	createAttachment(m_attachments[slot]);
}

void FrameBufferObject::setSize(int width, int height) 
{
	// Ensure one of the dimensions is not set to 0.
	m_specification.width = width ? width : 1;
	m_specification.height = height ? height : 1;
}
