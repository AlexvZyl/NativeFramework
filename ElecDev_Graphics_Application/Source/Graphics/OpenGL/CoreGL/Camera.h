#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "External/GLM/glm.hpp"

//==============================================================================================================================================//
//  Forward Declerations.																														//
//==============================================================================================================================================//

class Event;

//==============================================================================================================================================//
//  Camera Types.																																//
//==============================================================================================================================================//

enum class CameraType
{
	Standard2D, Standard3D
};

//==============================================================================================================================================//
//  Camera Class.																																//
//==============================================================================================================================================//

class Camera 
{
public:

	// Constructor.
	Camera(CameraType cameraType);

	// Updates the camera view matrix.
	void updateCamera();

	// Handles the passed event.
	void onEvent(Event& event);

private:

	friend class Renderer;
	friend class Scene;

	CameraType m_type;

	// ----------------- //
	//  C O N T R O L S  //
	// ----------------- //

	

	// ----------------- //
	//  M A T R I C E S  //
	// ----------------- //

	// MVP Matrices.
	glm::mat4 m_modelMatrix = glm::mat4(1.0f);					// The model matrix that places the object in the world.  Is going to be 
																// kept an identity matrix for now.
	glm::mat4 m_viewMatrix = glm::mat4(1.0f);					// The matrix that handles the camera movement.
																// viewMatrix = translatinMatrix * rotationMatrix * scalingMatrix;
	glm::mat4 m_projectionMatrix = glm::mat4(1.0f);				// The matrix that handles the clipping plane (which part of the world is
																// going to be visible to the screen?)
	glm::mat4 m_viewportMatrix = glm::mat4(1.0f);				// The matrix that handles the viewport transform.  Converts screen pixel
																// coordinates to the OpenGL uniform coordinate system.

	// View matrix components.
	glm::mat4 m_scalingMatrix = glm::mat4(1.0f);				// Handles camera scaling.
	glm::mat4 m_translationMatrix = glm::mat4(1.0f);			// Handles camera translations.
	glm::mat4 m_rotationMatrix = glm::mat4(1.0f);				// Handles camera rotations.

	// We need matrices to store the base view of the drawing.  This is to fall back to when right clicking, and this has to 
	// be updated with resizing and when auto sizing and scaling funtions are called.
	glm::mat4 m_scalingMatrixBase = glm::mat4(1.0f);			// Stores base matrix for camera scaling.
	glm::mat4 m_translationMatrixBase = glm::mat4(1.0f);		// Stores base matrix for camera translation.
	glm::mat4 m_rotationMatrixBase = glm::mat4(1.0f);			// Stores base matrix for camera rotation.
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
