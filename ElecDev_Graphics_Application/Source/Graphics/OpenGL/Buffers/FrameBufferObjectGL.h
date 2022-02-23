#pragma once

/*
These buffers are used to render OpenGL to a texture that can be used in various ways.
It is going to be used to render to an ImGUI widget.
[https://learnopengl.com/Advanced-OpenGL/Framebuffers]
*/

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <memory>
#include "GLM/glm.hpp"

//=============================================================================================================================================//
//  Forward declerations.																													   //
//=============================================================================================================================================//

template<typename VertexType>
class VertexArrayObject;

class VertexDataTextured;
class Shader;

//=============================================================================================================================================//
//  Class.																																	   //
//=============================================================================================================================================//

class FrameBufferObject 
{
public:

	// Saves the FBO size.
	glm::vec2 m_viewport = { 500, 500 };

	// Constructors.
	FrameBufferObject();
	FrameBufferObject(int width, int height, int MSAA);
	// Destructor.
	~FrameBufferObject();
	
	// Bind the FBO with the MSAA attachment.
	void bind();
	// Bind the FBO with the main render attachment.
	void bindRender();
	// Unbind the FBO.
	void unbind();
	// Get access to the render texture ID.
	unsigned int getRenderTexture();
	// Blit the MSAA texture to the Render texture.
	// This copies the multisampled FBO the render FBO, which is not multisampled,
	// but does have the processing effect.
	void blitFromMSAA();
	// This renders the texture from the MSAA attachment to a quad on the
	// render attachment.  the FBO does not have its own shader, so a static
	// shader has to be bound before hand.
	void renderFromMSAA();

	// Resizing the texure when the window resizes.
	void resize(int width, int height);
	// Clears the MSAA attachment of the FBO.
	void clear();
	// Clears the render attachment of the FBO.
	void clearRender();
	// Return the ID of the entity at the pixel coordinates.
	unsigned int getEntityID(glm::vec2& pixelCoords);


	// Initialise the color, depth and stencil attachments.
	void initAttachments(int width, int height);
	// Delete all of the attachments.
	void deleteResources();
	// Create and init the attachments (usually after a delete)
	void createResources(int width, int height);


private:

	// Friends.
	friend class Renderer;

	// FBO that will be rendered.
	unsigned int m_renderFrameBufferID;			// FBO ID.
	unsigned int m_renderColorTextureID;		// Texture ID.	
	unsigned int m_renderEntityIDTextureID;		// Texture containing the entity ID's.

	// MSAA FBO.
	unsigned int m_msaaFrameBufferID;			// FBO ID.
	unsigned int m_msaaColorTextureID;			// Texture ID.	
	unsigned int m_msaaDepthStencilBufferID;	// Depth/Stencil buffer.
	unsigned int m_msaaEntityIDTextureID;		// Texture containing the entity ID's.

	// The multisampling level (how many samples per pixel?).
	int m_MSAA = 1;

	// VAO that will render the quad to the render texture.
	std::unique_ptr<VertexArrayObject<VertexDataTextured>> m_renderVAO;

	// Shader to render the scene with.
	static std::unique_ptr<Shader> m_shader;

	// Has the resources been deleted?
	bool m_resourcesDeleted = true;
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//