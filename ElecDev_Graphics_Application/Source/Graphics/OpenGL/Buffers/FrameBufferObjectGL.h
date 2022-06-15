#pragma once

#include "GLM/glm.hpp"
#include "glad/glad.h"
#include <unordered_map>

enum class FrameBufferTextureFormat : GLenum
{
	// Raw formats.
	RGBA = GL_RGBA,
	RED_INTEGER = GL_RED_INTEGER,
	DEPTH_24_STENCIL_8 = GL_DEPTH24_STENCIL8,

	// Defaults.
	DEPTH = GL_DEPTH24_STENCIL8,
};

enum class FrameBufferTextureWrap : GLenum 
{
	CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
	CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
};

enum class FrameBufferTextureFilter : GLenum
{
	LINEAR = GL_LINEAR,
	NEAREST = GL_NEAREST
};

enum class FrameBufferAttachmentSlot : GLenum
{
	NONE = GL_NONE;

	// Color.
	COLOR_0 = GL_COLOR_ATTACHMENT0,
	COLOR_1 = GL_COLOR_ATTACHMENT1,
	COLOR_2 = GL_COLOR_ATTACHMENT2,
	COLOR_3 = GL_COLOR_ATTACHMENT3,
	COLOR_4 = GL_COLOR_ATTACHMENT4,
	COLOR_5 = GL_COLOR_ATTACHMENT5,
	COLOR_6 = GL_COLOR_ATTACHMENT6,
	COLOR_7 = GL_COLOR_ATTACHMENT7,
	COLOR_8 = GL_COLOR_ATTACHMENT8,
	COLOR_9 = GL_COLOR_ATTACHMENT9,
	COLOR_10 = GL_COLOR_ATTACHMENT10,
	COLOR_11 = GL_COLOR_ATTACHMENT11,
	COLOR_12 = GL_COLOR_ATTACHMENT12,

	// Other.
	DEPTH = GL_DEPTH_ATTACHMENT,
	DEPTH_STENCIL = GL_DEPTH_STENCIL_ATTACHMENT,
	STENCIl = GL_STENCIL_ATTACHMENT,
};

enum class FrameBufferAttachmentType 
{
	TEXTURE_BUFFER,
	TEXTURE_STORAGE,
	RENDER_BUFFER
};

enum class FrameBufferSamples : int
{
	NORMAL   = 1, 
	MSAA2    = 2, 
	MSAA4    = 4, 
	MSAA8    = 8, 
	MSAA16   = 16
};

struct FrameBufferAttachment 
{
	// Data.
	FrameBufferAttachmentType type = FrameBufferAttachmentType::TEXTURE_STORAGE;
	FrameBufferTextureFormat format = FrameBufferTextureFormat::RGBA;
	FrameBufferTextureFormat internalFormat = FrameBufferTextureFormat::RGBA;
	FrameBufferAttachmentSlot slot = FrameBufferAttachmentSlot::COLOR_0;
	FrameBufferTextureFilter minFilter = FrameBufferTextureFilter::LINEAR;
	FrameBufferTextureFilter magFilter = FrameBufferTextureFilter::LINEAR;
	FrameBufferSamples samples = FrameBufferSamples::NORMAL;
	FrameBufferTextureWrap wrapR = FrameBufferTextureWrap::CLAMP_TO_EDGE;
	FrameBufferTextureWrap wrapS = FrameBufferTextureWrap::CLAMP_TO_EDGE;
	FrameBufferTextureWrap wrapT = FrameBufferTextureWrap::CLAMP_TO_EDGE;
	unsigned rendererID = NULL;

	inline bool isMultiSample() const { return samples != FrameBufferSamples::NORMAL; }
};

struct FrameBufferSpecification 
{
	int width = 900;
	int height = 900;
};

class FrameBufferObject 
{
public:

	// Constructors.
	inline FrameBufferObject() = default;
	FrameBufferObject(const FrameBufferSpecification& spec) :m_specification(spec) { }

	// Destructor.
	~FrameBufferObject();

	// FBO functions.
	void create();
	void destroy();
	void resize();
	void resize(int width, int height);
	void bind();
	void unbind();

	// Attachments.
	void createAttachments();
	void clearAttachments();
	void destroyAttachments();
	void resizeAttachments();
	void createAttachment(FrameBufferAttachment& attachment);
	void createAttachment(FrameBufferAttachmentSlot slot);
	void resizeAttachment(const FrameBufferAttachment& attachment);
	void resizeAttachment(FrameBufferAttachmentSlot slot);
	void addAttachment(const FrameBufferAttachment& attachment);
	void removeAttachment(FrameBufferAttachmentSlot slot);
	void clearAttachment(FrameBufferAttachmentSlot slot);
	void clearAttachment(const FrameBufferAttachment& attachment);
	void destroyAttachment(FrameBufferAttachmentSlot slot);
	void destroyAttachment(const FrameBufferAttachment& attachment);

	// Utilities.
	inline auto& getSpecification() { return m_specification; }
	inline auto& getAttachments()   { return m_attachments;   }
	inline auto& getAttachment(FrameBufferAttachmentSlot slot) { return m_attachments[slot]; }
	inline void setSpecification(const FrameBufferSpecification& spec) { m_specification = spec; }
	inline void setDrawBuffers(const std::initializer_list<FrameBufferAttachmentSlot>& buffers) { m_drawBuffers = buffers; }
	inline void setReadBuffers(FrameBufferAttachmentSlot buffer) { m_readBuffer = buffer; }
	void bindDrawBuffers();
	void bindReadBuffer();

private:

	inline void attachmentsChanged() { m_attachmentsChanged = true; }

	// Data.
	FrameBufferSpecification m_specification;
	std::unordered_map<FrameBufferAttachmentSlot, FrameBufferAttachment> m_attachments;
	std::vector<FrameBufferAttachmentSlot> m_drawBuffers;
	FrameBufferAttachmentSlot m_readBuffer = FrameBufferAttachmentSlot::COLOR_0;
	unsigned m_rendererID = NULL;
	bool m_isOnGPU = false;
	bool m_attachmentsChanged = false;
};