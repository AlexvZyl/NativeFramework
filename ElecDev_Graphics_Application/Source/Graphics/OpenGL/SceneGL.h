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
	Scene(CameraType cameraType, const glm::vec2& size);

	// Destructor.
	virtual ~Scene();

	// Returns the rendered texture.
	unsigned getRenderTexture();

	// Returns the ID of the entity at the coordinates.
	unsigned getEntityID(const glm::vec2& pixelCoords);
	// Resizes the scene based on a viewport change.
	void resize(const glm::vec2& size);

	// Events.
	void onRenderInit();
	void onRenderCleanup();

	// Delete the resources to save on VRAM.
	void deleteGPUResources();
	// Recreate the resources once it has been deleted.
	void recreateGPUResources();

	// Getters.
	Camera& getCamera();
	Grid& getGrid();

private:

	// The grid drawn on the scene.
	std::unique_ptr<Grid> m_grid;

	friend class EngineCore;
	friend class Renderer;
	friend class Shader;
	friend class RendererStats;
	friend class BackgroundColorEditor;

	// The camera.
	std::unique_ptr<Camera> m_camera;

	// Map containing all of the different primitives.
	std::unordered_map<unsigned, std::unique_ptr<PrimitivePtr>> m_primitives;

	// Creates the default background based on the camera type.
	void createDefaultBackground();
	// Creates a default 2D background.
	void create2DBackground();
	// Creates a default 3D background.
	void create3DBackground();

	// Map containing all of the textures used in the scene.
	std::unordered_map<std::string, std::unique_ptr<Texture>> m_textures;
	// VAO's.
	std::unique_ptr<VertexArrayObject<VertexData>> m_backgroundVAO;
	std::unique_ptr<VertexArrayObject<VertexData, IndexData2>> m_linesVAO;
	std::unique_ptr<VertexArrayObject<VertexData>> m_trianglesVAO;
	std::unique_ptr<VertexArrayObject<VertexDataCircle>> m_circlesVAO;
	std::unique_ptr<VertexArrayObject<VertexDataTextured>> m_texturedTrianglesVAO;
	// FBO.
	std::unique_ptr<FrameBufferObject> m_FBO;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//