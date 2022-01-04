#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <vector>
#include <memory>
#include <map>
#include "External/GLM/glm.hpp"
#include <any>
#include "CoreGL/FrameBufferObjectGL.h"
#include "CoreGL/Texture.h"

//==============================================================================================================================================//
//  Forward Declerations.																														//
//==============================================================================================================================================//

template <typename VertexType>
class VertexArrayObject;
template <typename VertexType>
class Primitive;

class Camera;
class Texture;
class VertexData;
class VertexDataTextured;
class VertexDataCircle;
class Event;
class Entity;

enum class CameraType;

//==============================================================================================================================================//
//  Scene Class																																	//
//==============================================================================================================================================//

class Scene
{
public:

	// Constructor.
	Scene(CameraType cameraType, float width, float height, unsigned msaaSamples = 8);
	// Set the MSAA samples.
	void setMSAA(unsigned samples);
	// Pass events on to the camera.
	void onEvent(Event& event);
	// Returns the rendered texture.
	unsigned getRenderTexture();
	// Set the viewport dimensions.
	void setViewport(int width, int height);
	// Returns the viewport dimensions.
	glm::vec2 getViewport();
	// Calculate the world coordinates from the pixel coordinates.
	glm::vec3 pixelCoordsToWorldCoords(float pixelCoords[2]);
	// Calculate the camera coordinates from the pixel coordinates.
	glm::vec3 pixelCoordsToCameraCoords(float pixelCoords[2]);
	// Returns the ID of the entity at the coordinates.
	unsigned getEntityID(glm::vec2& pixelCoords);

private:

	// Friends.
	friend class Renderer;

	// Background methods.
	void createDefaultBackground();
	void create2DBackground();
	void create3DBackground();

	// Return the view matrix of the scene camera.
	glm::mat4* getViewMatrix();
	// Return the projection matrix.
	glm::mat4* getProjectionMatrix();
	// Upadate the matrices of the camera.
	void updateCamera();

	void resize();

	// Data required to construct a scene in OpenGL.
	std::unique_ptr<Camera> m_camera;
	// Map containing all of the different primitives.
	std::map<unsigned, std::unique_ptr<Entity>> m_primitives;
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