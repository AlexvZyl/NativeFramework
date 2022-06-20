#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "OpenGL/Buffers/FrameBufferObjectGL.h"
#include "OpenGL/Buffers/GraphicsPrimitivesBuffersGL.h"
#include "OpenGL/Primitives/TextureGL.h"
#include "OpenGL/Primitives/Primitive.h"
#include "Graphics/Camera/Camera.h"

#include <vector>
#include <memory>
#include <unordered_map>
#include "glm/glm.hpp"

//==============================================================================================================================================//
//  Forward Declerations.																														//
//==============================================================================================================================================//

class VertexData;
class VertexDataTextured;
class VertexDataCircle; 
class Event;
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

	// Get the texture that has to be rendered to display the scene.
	// Does not change based on MSAA / non-MSAA, but does change when resources
	// gets destroyed.
	unsigned getRenderTexture();

	// Returns the ID of the entity at the coordinates.
	unsigned getEntityID(const glm::vec2& pixelCoords);

	// Resize the scene.
	// Passed on to the render targets and the camera.
	void resize(const glm::vec2& size);

	// Delete the resources to save on VRAM.
	void deleteGPUResources();
	// Recreate the resources once it has been deleted.
	void recreateGPUResources();

	// Getters.
	inline Camera& getCamera() { return m_camera; }
	inline Grid& getGrid() { return *m_grid.get(); }

private:

	friend class EngineCore;
	friend class Renderer;
	friend class Shader;
	friend class RendererStats;
	friend class BackgroundColorEditor;
	friend class SettingsWidget;

	// Creates the default background based on the camera type.
	void createDefaultBackground();
	// Creates a default 2D background.
	void create2DBackground();
	// Creates a default 3D background.
	void create3DBackground();

	// Map containing all of the textures used in the scene.
	// TODO: This should rather be handled by a resource manager.
	std::unordered_map<std::string, std::unique_ptr<Texture>> m_textures;

	// The grid drawn on the scene.
	// Can be disabled by the engine.
	std::unique_ptr<Grid> m_grid;

	// The camera.
	Camera m_camera;

	// Graphics Buffers.
	// TODO: These have to be dynamic in some way.
	// Just use vectors and iterate them?  Use RenderCommands?
	GraphicsLinesBuffer<VertexData> m_linesBuffer;
	GraphicsTrianglesBuffer<VertexData> m_backgroundBuffer;
	GraphicsTrianglesBuffer<VertexData> m_trianglesBuffer;
	GraphicsTrianglesBuffer<VertexDataCircle> m_circlesBuffer;
	GraphicsTrianglesBuffer<VertexDataTextured> m_texturedTrianglesBuffer;

	// Render targets.
	FrameBufferObject m_msaaFBO;
	FrameBufferObject m_renderFBO;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//