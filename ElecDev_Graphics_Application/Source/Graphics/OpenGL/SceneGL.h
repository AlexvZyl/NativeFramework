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

class Grid;

//==============================================================================================================================================//
//  Scene Class																																	//
//==============================================================================================================================================//

class Scene
{
public:

	// Constructor.
	Scene(CameraType cameraType, float width, float height);
	// Destructor.
	virtual ~Scene();
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
	void resize(const glm::vec2& size);

	void onRenderInit();
	void onRenderCleanup();

	// Delete the resources to save on VRAM.
	void deleteGPUResources();
	// Recreate the resources once it has been deleted.
	void recreateGPUResources();

	// Update the scene.
	void onUpdate();

	// The camera.
	std::unique_ptr<Camera> m_camera;

	// The grid drawn on the scene.
	std::unique_ptr<Grid> m_grid;

private:

	friend class EngineCore;

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
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//