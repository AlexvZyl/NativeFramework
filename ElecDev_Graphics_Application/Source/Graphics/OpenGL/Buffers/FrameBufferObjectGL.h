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
	R32_UI = GL_R32UI,

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
	NONE = GL_NONE,

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
	COLOR_13 = GL_COLOR_ATTACHMENT13,
	COLOR_14 = GL_COLOR_ATTACHMENT14,
	COLOR_15 = GL_COLOR_ATTACHMENT15,
	COLOR_16 = GL_COLOR_ATTACHMENT16,
	COLOR_17 = GL_COLOR_ATTACHMENT17,
	COLOR_18 = GL_COLOR_ATTACHMENT18,
	COLOR_19 = GL_COLOR_ATTACHMENT19,
	COLOR_20 = GL_COLOR_ATTACHMENT20,
	COLOR_21 = GL_COLOR_ATTACHMENT21,
	COLOR_22 = GL_COLOR_ATTACHMENT22,
	COLOR_23 = GL_COLOR_ATTACHMENT23,
	COLOR_24 = GL_COLOR_ATTACHMENT24,
	COLOR_25 = GL_COLOR_ATTACHMENT25,
	COLOR_26 = GL_COLOR_ATTACHMENT26,
	COLOR_27 = GL_COLOR_ATTACHMENT27,
	COLOR_28 = GL_COLOR_ATTACHMENT28,
	COLOR_29 = GL_COLOR_ATTACHMENT29,
	COLOR_30 = GL_COLOR_ATTACHMENT30,
	COLOR_31 = GL_COLOR_ATTACHMENT31,

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
	MSAA16   = 16,
	MSAA32	 = 32
};

struct FrameBufferAttachment 
{
	// Data.
	FrameBufferAttachmentType type = FrameBufferAttachmentType::TEXTURE_BUFFER;
	FrameBufferAttachmentSlot slot = FrameBufferAttachmentSlot::COLOR_0;
	FrameBufferTextureFormat format = FrameBufferTextureFormat::RGBA;
	FrameBufferTextureFormat internalFormat = FrameBufferTextureFormat::RGBA;
	FrameBufferTextureFilter minFilter = FrameBufferTextureFilter::LINEAR;
	FrameBufferTextureFilter magFilter = FrameBufferTextureFilter::LINEAR;
	FrameBufferTextureWrap wrapR = FrameBufferTextureWrap::CLAMP_TO_EDGE;
	FrameBufferTextureWrap wrapS = FrameBufferTextureWrap::CLAMP_TO_EDGE;
	FrameBufferTextureWrap wrapT = FrameBufferTextureWrap::CLAMP_TO_EDGE;
	FrameBufferSamples samples = FrameBufferSamples::NORMAL;
	unsigned rendererID = NULL;
	bool created = false;
	// Utilities.
	inline bool isMultiSample() const { return (int)samples > 1; }
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
	FrameBufferObject(const FrameBufferSpecification& spec) : m_specification(spec) { }

	// Destructor.
	~FrameBufferObject();

	// FBO functions.
	void create();
	void clear();
	void create(int width, int height);
	void destroy();
	void resize(int width, int height);
	void bind();
	void unbind();

	// Utilities.
	inline auto& getSpecification() { return m_specification; }
	inline auto& getAttachments()   { return m_attachments;   }
	inline auto& getAttachment(FrameBufferAttachmentSlot slot) { return m_attachments[slot]; }
	inline void setSpecification(const FrameBufferSpecification& spec) { m_specification = spec; }
	inline unsigned getID() const { return m_rendererID; }

	// Attachments.
	inline void setDrawBuffers(const std::initializer_list<FrameBufferAttachmentSlot>& buffers) { m_drawBuffers = buffers; }
	inline void setReadBuffer(FrameBufferAttachmentSlot buffer) { m_readBuffer = buffer; }
	void bindDrawBuffers();
	void bindReadBuffer();
	void addAttachment(const FrameBufferAttachment& attachment);
	void removeAttachment(FrameBufferAttachmentSlot slot);
	void clearAttachments(int value = 0);
	void clearAttachment(FrameBufferAttachmentSlot slot, int value = 0);
	int readPixel(FrameBufferAttachmentSlot slot, int x, int y);

private:

	// Attachments.
	void createAttachments();
	void destroyAttachments();
	void resizeAttachments();
	void createAttachment(FrameBufferAttachment& attachment);
	void createAttachment(FrameBufferAttachmentSlot slot);
	void resizeAttachment(const FrameBufferAttachment& attachment);
	void resizeAttachment(FrameBufferAttachmentSlot slot);
	void destroyAttachment(FrameBufferAttachmentSlot slot);
	void destroyAttachment(FrameBufferAttachment& attachment);
	inline void attachmentsChanged() { m_attachmentsChanged = true; }
	void clearAttachment(const FrameBufferAttachment& attachment, int value = 0);

	// Data.
	FrameBufferSpecification m_specification;
	std::unordered_map<FrameBufferAttachmentSlot, FrameBufferAttachment> m_attachments;
	std::vector<FrameBufferAttachmentSlot> m_drawBuffers;
	FrameBufferAttachmentSlot m_readBuffer = FrameBufferAttachmentSlot::COLOR_0;
	unsigned m_rendererID = NULL;
	bool m_isOnGPU = false;
	bool m_attachmentsChanged = false;
};