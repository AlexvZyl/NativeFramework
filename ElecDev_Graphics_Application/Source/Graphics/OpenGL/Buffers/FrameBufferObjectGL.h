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
	FrameBufferObject(int width, int height);
	// Destructor.
	~FrameBufferObject();
	
	// Bind the FBO with the MSAA attachment.
	void bind();
	// Unbind the FBO.
	void unbind();
	// Clear the FBO and its attachments.
	void clear();
	// Get access to the render texture ID.
	unsigned int getRenderTexture();
	// Resizing the texure when the window resizes.
	void resize(int width, int height);
	// Return the ID of the entity at the pixel coordinates.
	unsigned int getEntityID(const glm::vec2& pixelCoords);
	// Sets the attachments to be drawn to in the shaders.
	void setDrawBuffers();

	// Initialise the color, depth and stencil attachments.
	void initAttachments(int width, int height);
	// Create and init the attachments (usually after a delete)
	void createResources(int width, int height);
	// Delete all of the attachments.
	void deleteResources();

private:

	// Friends.
	friend class Renderer;
	friend class Scene;

	// FBO that will be rendered.
	unsigned int m_frameBufferID;		// FBO ID.
	unsigned int m_colorTextureID;		// Texture ID.	
	unsigned int m_entityIDTextureID;	// Texture containing the entity ID's.
	unsigned int m_depthStencilID;		
	// Outline.
	unsigned int m_outlineColorTextureID;

	// Has the resources been deleted?
	bool m_resourcesDeleted = true;
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//