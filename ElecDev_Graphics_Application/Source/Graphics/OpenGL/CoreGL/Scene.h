#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <vector>
#include <memory>
#include <map>
#include "External/GLM/glm.hpp"

//==============================================================================================================================================//
//  Forward Declerations.																														//
//==============================================================================================================================================//

template <typename VertexType>
class VertexArrayObject;
template <typename VertexType>
class Primitive;

class Camera;
class FrameBufferObject;
class Texture;
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
	// Resize the scene.
	void onResizeEvent(float width, float height);

	glm::vec2 m_viewport;
	// Data required to construct a scene in OpenGL.
	std::unique_ptr<Camera> m_camera;
	std::map<unsigned, std::unique_ptr<Primitive>> m_primitives;
	std::map<unsigned, std::unique_ptr<Texture>> m_textures;
	std::unique_ptr<VertexArrayObject<VertexData>> m_linesVAO;
	std::unique_ptr<VertexArrayObject<VertexData>> m_trianglesVAO;
	std::unique_ptr<VertexArrayObject<VertexDataCircle>> m_circlesVAO;
	std::unique_ptr<VertexArrayObject<VertexDataTextured>> m_texturedTrianglesVAO;
	std::unique_ptr<VertexArrayObject<VertexDataTextured>> m_backgroundVAO;
	std::unique_ptr<FrameBufferObject> m_FBO;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//