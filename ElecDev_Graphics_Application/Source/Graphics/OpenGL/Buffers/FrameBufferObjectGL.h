#pragma once

#include "GLM/glm.hpp"
#include "glad/glad.h"
#include <unordered_map>
#include <string>
#include "Utilities/Assert/Assert.h"

enum class FrameBufferTextureFormat : GLenum
{
	// Raw formats.
	RGBA = GL_RGBA,
	RGBA8 = GL_RGBA8,
	RED_INTEGER = GL_RED_INTEGER,
	DEPTH_24_STENCIL_8 = GL_DEPTH24_STENCIL8,
	R32_UI = GL_R32UI,
	DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
	DEPTH_24 = GL_DEPTH_COMPONENT24,
	DEPTH_32 = GL_DEPTH_COMPONENT32,
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
	STENCIL = GL_STENCIL_ATTACHMENT,
};

enum class FrameBufferAttachmentType 
{
	TEXTURE_BUFFER,
	TEXTURE_STORAGE,
	RENDER_BUFFER
};

enum class FrameBufferSamples : int
{
	NORMAL   = 0, 
	MSAA1	 = 1,
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

	// Is it a multisampled attachment?
	inline bool isMultiSample() const { return (int)samples > 0; }

	// Get the slot as a string.
	std::string slotString() const
	{
		switch (slot)
		{
		case FrameBufferAttachmentSlot::NONE:		   return "GL_NONE";
		case FrameBufferAttachmentSlot::COLOR_0:	   return "GL_COLOR_ATTACHMENT0";
		case FrameBufferAttachmentSlot::COLOR_1:	   return "GL_COLOR_ATTACHMENT1";
		case FrameBufferAttachmentSlot::COLOR_2:	   return "GL_COLOR_ATTACHMENT2";
		case FrameBufferAttachmentSlot::COLOR_3:	   return "GL_COLOR_ATTACHMENT3";
		case FrameBufferAttachmentSlot::COLOR_4:	   return "GL_COLOR_ATTACHMENT4";
		case FrameBufferAttachmentSlot::COLOR_5:	   return "GL_COLOR_ATTACHMENT5";
		case FrameBufferAttachmentSlot::COLOR_6:	   return "GL_COLOR_ATTACHMENT6";
		case FrameBufferAttachmentSlot::COLOR_7:	   return "GL_COLOR_ATTACHMENT7";
		case FrameBufferAttachmentSlot::COLOR_8:	   return "GL_COLOR_ATTACHMENT8";
		case FrameBufferAttachmentSlot::COLOR_9:	   return "GL_COLOR_ATTACHMENT9";
		case FrameBufferAttachmentSlot::COLOR_10:	   return "GL_COLOR_ATTACHMENT10";
		case FrameBufferAttachmentSlot::COLOR_11:	   return "GL_COLOR_ATTACHMENT11";
		case FrameBufferAttachmentSlot::COLOR_12:	   return "GL_COLOR_ATTACHMENT12";
		case FrameBufferAttachmentSlot::COLOR_13:	   return "GL_COLOR_ATTACHMENT13";
		case FrameBufferAttachmentSlot::COLOR_14:	   return "GL_COLOR_ATTACHMENT14";
		case FrameBufferAttachmentSlot::COLOR_15:	   return "GL_COLOR_ATTACHMENT15";
		case FrameBufferAttachmentSlot::COLOR_16:	   return "GL_COLOR_ATTACHMENT16";
		case FrameBufferAttachmentSlot::COLOR_17:	   return "GL_COLOR_ATTACHMENT17";
		case FrameBufferAttachmentSlot::COLOR_18:	   return "GL_COLOR_ATTACHMENT18";
		case FrameBufferAttachmentSlot::COLOR_19:	   return "GL_COLOR_ATTACHMENT19";
		case FrameBufferAttachmentSlot::COLOR_20:	   return "GL_COLOR_ATTACHMENT20";
		case FrameBufferAttachmentSlot::COLOR_21:	   return "GL_COLOR_ATTACHMENT21";
		case FrameBufferAttachmentSlot::COLOR_22:	   return "GL_COLOR_ATTACHMENT22";
		case FrameBufferAttachmentSlot::COLOR_23:	   return "GL_COLOR_ATTACHMENT23";
		case FrameBufferAttachmentSlot::COLOR_24:	   return "GL_COLOR_ATTACHMENT24";
		case FrameBufferAttachmentSlot::COLOR_25:	   return "GL_COLOR_ATTACHMENT25";
		case FrameBufferAttachmentSlot::COLOR_26:	   return "GL_COLOR_ATTACHMENT26";
		case FrameBufferAttachmentSlot::COLOR_27:	   return "GL_COLOR_ATTACHMENT27";
		case FrameBufferAttachmentSlot::COLOR_28:	   return "GL_COLOR_ATTACHMENT28";
		case FrameBufferAttachmentSlot::COLOR_29:	   return "GL_COLOR_ATTACHMENT29";
		case FrameBufferAttachmentSlot::COLOR_30:	   return "GL_COLOR_ATTACHMENT30";
		case FrameBufferAttachmentSlot::COLOR_31:	   return "GL_COLOR_ATTACHMENT31";
		case FrameBufferAttachmentSlot::DEPTH:		   return "GL_DEPTH_ATTACHMENT";
		case FrameBufferAttachmentSlot::STENCIL:	   return "GL_STENCIL_ATTACHMENT";
		case FrameBufferAttachmentSlot::DEPTH_STENCIL: return "GL_DEPTH_STENCIL_ATTACHMENT";
		default: LUMEN_ASSERT(false, "Unknown attachment.");
		}
	}

	// Get the data type based one the format.
	// TODO: FIX THIS!
	GLenum dataType() const
	{
		switch (format) 
		{
		case FrameBufferTextureFormat::RGBA:				return GL_FLOAT;
		case FrameBufferTextureFormat::RGBA8:				return GL_FLOAT;
		case FrameBufferTextureFormat::DEPTH_24_STENCIL_8:	return GL_FLOAT;
		case FrameBufferTextureFormat::R32_UI:				return GL_UNSIGNED_INT;
		case FrameBufferTextureFormat::RED_INTEGER:			return GL_UNSIGNED_INT;
		case FrameBufferTextureFormat::DEPTH_COMPONENT:		return GL_DEPTH_COMPONENT;
		case FrameBufferTextureFormat::DEPTH_24:		    return GL_UNSIGNED_INT;
		case FrameBufferTextureFormat::DEPTH_32:		    return GL_UNSIGNED_INT;
		default: LUMEN_ASSERT(false, "Unknown format.");
		}
	}
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

	// General.
	void create();
	void create(int width, int height);
	void resize(int width, int height);
	void destroy();
	void bind();
	void unbind();

	// Utilities.
	inline auto& getSpecification() { return m_specification; }
	inline void setSpecification(const FrameBufferSpecification& spec) { m_specification = spec; }
	inline unsigned getID() const { return m_rendererID; }

	// Attachments.
	void addAttachment(const FrameBufferAttachment& attachment);
	void removeAttachment(FrameBufferAttachmentSlot slot);
	inline auto& getAttachments() { return m_attachments; }
	inline auto& getAttachment(FrameBufferAttachmentSlot slot) { return m_attachments[slot]; }
	inline void setDrawBuffers(const std::initializer_list<FrameBufferAttachmentSlot>& buffers) { m_drawBuffers = buffers; }
	inline void setDrawBuffers(const std::vector<FrameBufferAttachmentSlot>& buffers) { m_drawBuffers = buffers; }
	inline void setReadBuffer(FrameBufferAttachmentSlot buffer) { m_readBuffer = buffer; }
	void bindDrawBuffers();
	void bindReadBuffer();
	int readPixel(FrameBufferAttachmentSlot slot, int x, int y);

	// For now refrain from using this function.  Clearing attachments is buggy.
	// Rather use `Renderer::clearAll()` after binding.
	void clear();
	// For now refrain from using this function.  Clearing attachments is buggy.
	// Rather use `Renderer::clearAll()` after binding.
	void clearAttachment(FrameBufferAttachmentSlot slot);

private:

	friend class SettingsWidget;

	// Attachments.
	void clearAttachments();
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
	void clearAttachment(const FrameBufferAttachment& attachment);

	// Data.
	FrameBufferSpecification m_specification;
	std::unordered_map<FrameBufferAttachmentSlot, FrameBufferAttachment> m_attachments;
	std::vector<FrameBufferAttachmentSlot> m_drawBuffers;
	FrameBufferAttachmentSlot m_readBuffer = FrameBufferAttachmentSlot::COLOR_0;
	unsigned m_rendererID = NULL;
	bool m_isOnGPU = false;
	bool m_attachmentsChanged = false;
};
