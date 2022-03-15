#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <vector>
#include <memory>
#include <unordered_map>
#include "glm/glm.hpp"
#include "OpenGL/Buffers/FrameBufferObjectGL.h"
#include "OpenGL/Buffers/VertexArrayObjectGL.h"
#include "OpenGL/Primitives/TextureGL.h"
#include "OpenGL/Primitives/Primitive.h"
#include "Graphics/Camera/Camera.h"

//==============================================================================================================================================//
//  Forward Declerations.																														//
//==============================================================================================================================================//

class VertexData;
class VertexDataTextured;
class VertexDataCircle; 
class Event;

enum class CameraType;

//==============================================================================================================================================//
//  Scene Class																																	//
//==============================================================================================================================================//

class Scene
{
public:

	// Constructor.
	Scene(CameraType cameraType, float width, float height, unsigned msaaSamples = 16);
	// Destructor.
	virtual ~Scene();
	// Set the MSAA samples.
	void setMSAA(unsigned samples);
	// Handle events.
	void onEvent(Event& event);
	// Returns the rendered texture.
	unsigned getRenderTexture();
	// Set the viewport dimensions.
	void setViewport(int width, int height);
	// Returns the viewport dimensions.
	glm::vec4& getViewport();
	// Calculate the world coordinates from the pixel coordinates.
	glm::vec3 pixelCoordsToWorldCoords(const glm::vec2& pixelCoords);
	// Calculate the camera coordinates from the pixel coordinates.
	glm::vec3 pixelCoordsToCameraCoords(const glm::vec2& pixelCoords);
	// Returns the ID of the entity at the coordinates.
	unsigned getEntityID(const glm::vec2& pixelCoords);
	// Resizes the scene based on a viewport change.
	void resize(int width, int heigth);

	// Delete the resources to save on VRAM.
	void deleteGPUResources();
	// Recreate the resources once it has been deleted.
	void recreateGPUResources();

	// Update the scene.
	void onUpdate();

	// The camera.
	std::unique_ptr<Camera> m_camera;

private:

	// Map containing all of the different primitives.
	std::unordered_map<unsigned, std::unique_ptr<PrimitivePtr>> m_primitives;

	// Friends.
	friend class Renderer;
	friend class Shader;
	friend class RendererStats;
	friend class BackgroundColorEditor;

	// Creates the default background based on the camera type.
	void createDefaultBackground();
	// Creates a default 2D background.
	void create2DBackground();
	// Creates a default 3D background.
	void create3DBackground();

	// Binds the FBO.
	void bindFBO();
	// Clears the FBO.
	void clearFBO();
	// Unbinds the FBO.
	void unbindFBO();

	// Return the view matrix of the scene camera.
	glm::mat4* getViewMatrix();
	// Return the projection matrix.
	glm::mat4* getProjectionMatrix();

	// Map containing all of the textures used in the scene.
	std::unordered_map<std::string, std::unique_ptr<Texture>> m_textures;
	// VAO's.
	std::unique_ptr<VertexArrayObject<VertexData>> m_backgroundVAO;
	std::unique_ptr<VertexArrayObject<VertexData>> m_linesVAO;
	std::unique_ptr<VertexArrayObject<VertexData>> m_trianglesVAO;
	std::unique_ptr<VertexArrayObject<VertexDataCircle>> m_circlesVAO;
	std::unique_ptr<VertexArrayObject<VertexDataTextured>> m_texturedTrianglesVAO;
	// FBO.
	std::unique_ptr<FrameBufferObject> m_FBO;
	
	// --------------------- //
	//  B A C K G R O U N D  //
	// --------------------- //

	float m_colorScaleDark = 1.0f;
	float m_colorScaleLight = 3.f;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//