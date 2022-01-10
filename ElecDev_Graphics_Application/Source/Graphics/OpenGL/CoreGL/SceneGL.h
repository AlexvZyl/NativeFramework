#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <vector>
#include <memory>
#include <map>
#include "External/GLM/glm.hpp"
#include <any>
#include "CoreGL/Buffers/FrameBufferObjectGL.h"
#include <variant>
#include "CoreGL/TextureGL.h"
#include "CoreGL/Entities/Primitive.h"

//==============================================================================================================================================//
//  Forward Declerations.																														//
//==============================================================================================================================================//

template <typename VertexType>
class VertexArrayObject;

class Camera;
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
	glm::vec3 pixelCoordsToWorldCoords(float pixelCoords[2]);
	// Calculate the camera coordinates from the pixel coordinates.
	glm::vec3 pixelCoordsToCameraCoords(float pixelCoords[2]);
	// Returns the ID of the entity at the coordinates.
	unsigned getEntityID(glm::vec2& pixelCoords);
	// Resizes the scene based on a viewport change.
	void resize(int width, int heigth);

private:

	// Map containing all of the different primitives.
	std::map<unsigned, std::unique_ptr<PrimitivePtr>> m_primitives;

	// Friends.
	friend class Renderer;
	friend class Shader;

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
	// Upadate the matrices of the camera.
	void updateCamera();

	// The camera.
	std::unique_ptr<Camera> m_camera;
	// Map containing all of the textures used in the scene.
	std::map<unsigned, std::unique_ptr<Texture>> m_textures;
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