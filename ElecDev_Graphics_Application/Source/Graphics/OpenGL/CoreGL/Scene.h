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

private:

	friend class Renderer;

	// Return the view matrix of the scene camera.
	glm::mat4* getViewMatrix();
	// Upadate the matrices of the camera.
	void updateCamera();
	// Get the viewport dimensions.
	glm::vec2* getViewportDimensions();

	// Data required to construct a scene in OpenGL.
	std::unique_ptr<Camera> m_camera;
	// Map containing all of the different primitives.
	std::map<unsigned, std::unique_ptr<Entity>> m_primitives;
	// Map containing all of the textures used in the scene.
	std::map<unsigned, std::unique_ptr<Texture>> m_textures;
	// VAO's.
	std::unique_ptr<VertexArrayObject<VertexData>> m_linesVAO;
	std::unique_ptr<VertexArrayObject<VertexData>> m_trianglesVAO;
	std::unique_ptr<VertexArrayObject<VertexDataCircle>> m_circlesVAO;
	std::unique_ptr<VertexArrayObject<VertexDataTextured>> m_texturedTrianglesVAO;
	std::unique_ptr<VertexArrayObject<VertexDataTextured>> m_backgroundVAO;
	// FBO.
	std::unique_ptr<FrameBufferObject> m_FBO;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//