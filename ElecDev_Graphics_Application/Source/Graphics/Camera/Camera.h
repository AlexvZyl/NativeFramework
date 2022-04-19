#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "glm/glm.hpp"
#include <array>

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
	Camera(CameraType cameraType, const glm::vec2& size);
	// Updates the camera components.
	void onUpdate();
	// Set the size of the viewport (assuming a start at 0,0).
	void setViewport(const glm::vec2& viewport);
	// Set the size of the viewport.
	void setViewport(const glm::vec4& viewport);
	// Get the viewport.
	const glm::vec4& getViewport() const;
	// Get the viewport size.
	const glm::vec2& getViewportSize() const;
	// Resize the camera (viewport).
	void resize(const glm::vec2& size);

	// Calculate the world coordinates from the pixel coordinates.
	glm::vec3 pixelCoordsToWorldCoords(const glm::vec2& pixelCoords);
	// Calculate the camera coordinates from the pixel coordinates.
	glm::vec3 pixelCoordsToCameraCoords(const glm::vec2& pixelCoords);

	// Get the type of camera.
	const CameraType& getType() const;

	// Get matrices.
	const glm::mat4& getViewMatrix() const;
	const glm::mat4& getProjectionMatrix() const;


	// ----------------- //
	//  C O N T R O L S  //
	// ----------------- //

	void translate(const glm::vec3& translation);
	void translate(const glm::vec2& translation);


private:

	CameraType m_type;

	// Construct camera types.
	void construct2DCamera(const glm::vec2& size);
	void construct3DCamera(const glm::vec2& size);
	void moved();

	// ----------------- //
	//  M A T R I C E S  //
	// ----------------- //

	// MVP Matrices.
	glm::mat4 m_viewMatrix = glm::mat4(1.0f);					// The matrix that handles the camera movement.
																// viewMatrix = translatinMatrix * rotationMatrix * scalingMatrix;
	glm::mat4 m_projectionMatrix = glm::mat4(1.0f);				// The matrix that handles the clipping plane (which part of the world is
																// going to be visible to the screen?)
	glm::vec4 m_viewportVec = glm::vec4(1.0f);					// The vector that handles the viewport transform.  Converts screen pixel
																// coordinates to the OpenGL uniform coordinate system.
	// View matrix components.W
	glm::mat4 m_scalingMatrix = glm::mat4(1.0f);				// Handles camera scaling.
	glm::mat4 m_translationMatrix = glm::mat4(1.0f);			// Handles camera translations.
	glm::mat4 m_rotationMatrix = glm::mat4(1.0f);				// Handles camera rotations.

	// We need matrices to store the base view of the drawing.  This is to fall back to when restoring to home view, and this has to 
	// be updated with resizing and when auto sizing and scaling funtions are called.
	glm::mat4 m_scalingMatrixHome = glm::mat4(1.0f);			// Stores base matrix for camera scaling.
	glm::mat4 m_translationMatrixHome = glm::mat4(1.0f);		// Stores base matrix for camera translation.
	glm::mat4 m_rotationMatrixHome = glm::mat4(1.0f);			// Stores base matrix for camera rotation.
	std::array<float, 6> m_projectionValues;					// Stores the valaues that is used to calculate the projection matrix.
	float m_scaleRate = 0.2f;									// How fast the camera scales.
	bool m_viewMatrixUpToDate = true;							// Is the viewmatrix up to date with the latest changes?
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
