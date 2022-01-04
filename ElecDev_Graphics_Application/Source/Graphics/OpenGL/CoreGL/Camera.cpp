//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"

//==============================================================================================================================================//
//  Functions.																																	//
//==============================================================================================================================================//

Camera::Camera(CameraType cameraType, int width, int height) 
	: m_type(cameraType)
{
	setViewport(width, height);
}

void Camera::updateCamera() 
{
	m_viewMatrix = m_scalingMatrix * m_rotationMatrix * m_translationMatrix; 
}

//==============================================================================================================================================//
//  Controls.																																	//
//==============================================================================================================================================//



//==============================================================================================================================================\//
//  Coordinate systems.																															//
//==============================================================================================================================================//

void Camera::setViewport(int width, int height) 
{
	m_viewportVec[2] = (float)width;
	m_viewportVec[3] = (float)height;
}

glm::vec3 Camera::pixelCoordsToWorldCoords(float pixelCoords[2])
{
	glm::vec3 pixelCoords3 = { pixelCoords[0], m_viewportVec[3] - pixelCoords[1], 0.0f };
	m_viewMatrix = m_scalingMatrix * m_rotationMatrix * m_translationMatrix;
	glm::vec3 worldVec = glm::unProject(pixelCoords3, m_viewMatrix, m_projectionMatrix, m_viewportVec);
	return worldVec;
}

glm::vec3 Camera::pixelCoordsToCameraCoords(float pixelCoords[2])
{
	// Find the viewpwort dimensions.
	float viewport[2] = { m_viewportVec[2], m_viewportVec[3]};
	// Account for pixel offset.
	float viewportOffset[2] = { (float)viewport[0], (float)viewport[1] };
	// OpenGL places the (0,0) point in the top left of the screen.  Place it in the bottom left cornder.
	float pixelCoordsTemp[2] = { pixelCoords[0] , (float)viewport[1] - pixelCoords[1] };
	// The nomalized mouse coordinates on the users screen.
	float normalizedScreenCoords[2];
	// Apply the viewport transform the the pixels.
	normalizedScreenCoords[0] = (pixelCoordsTemp[0] - viewportOffset[0] / 2) / (viewportOffset[0] / 2);
	normalizedScreenCoords[1] = (pixelCoordsTemp[1] - viewportOffset[1] / 2) / (viewportOffset[1] / 2);
	// Convert to screen vector.
	glm::vec4 screenVec = { normalizedScreenCoords[0], normalizedScreenCoords[1], 0.0f, 1.0f };
	// Apply MVP matrices.
	m_viewMatrix = m_scalingMatrix * m_rotationMatrix * m_translationMatrix;
	glm::mat4 MVPinverse = glm::inverse(m_modelMatrix * m_viewMatrix * m_projectionMatrix);
	glm::vec4 worldVec = screenVec * MVPinverse;
	return worldVec;
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
