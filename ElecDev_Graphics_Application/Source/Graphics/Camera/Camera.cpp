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

	// --------------------------------- //
	//  P R O J E C T I O N   S E T U P  //
	// --------------------------------- //

	// Find the minimum value of the viewport dimensions.
	float minValue;
	if (m_viewportVec[2] < m_viewportVec[3]) { minValue = m_viewportVec[2]; }
	else { minValue = m_viewportVec[3]; }
	// Scale the projection values according to the ImGUI viewport.
	float projValuesTemp[6] = { (float)-m_viewportVec[2] / minValue, (float)m_viewportVec[2] / minValue, (float)-m_viewportVec[3] / minValue, (float)m_viewportVec[3] / minValue,-1.0, 1.0 };

	// Save projection values to be used with resizing of the window.
	for (int i = 0; i < 6; i++) { m_projectionValues[i] = projValuesTemp[i]; }
	// Create projection matrix.
	m_projectionValues[4] = -1.0f;
	m_projectionValues[5] = 1.0f;
	m_projectionMatrix = glm::ortho(m_projectionValues[0], m_projectionValues[1], m_projectionValues[2], m_projectionValues[3], m_projectionValues[4], m_projectionValues[5]);
}

void Camera::updateCamera() 
{
	m_viewMatrix = m_scalingMatrix * m_rotationMatrix * m_translationMatrix; 
}

void Camera::resize(int width, int height) 
{
	// Calculate the value of the scaling.
	float scalingFactor[2] = { (float)width / (float)m_viewportVec[2], (float)height / (float)m_viewportVec[3]};
	m_viewportVec[2] = (float)width;
	m_viewportVec[3] = (float)height;

	// Scale projection values.
	m_projectionValues[0] *= scalingFactor[0];
	m_projectionValues[1] *= scalingFactor[0];
	m_projectionValues[2] *= scalingFactor[1];
	m_projectionValues[3] *= scalingFactor[1];

	// Scale with the y scaling.
	//m_scalingMatrix = glm::scale(m_scalingMatrix, glm::vec3(scalingFactor[1], scalingFactor[1], 1.0f));
	// Update base matrix.
	m_scalingMatrixBase = glm::scale(m_scalingMatrixBase, glm::vec3(scalingFactor[1], scalingFactor[1], 1.0f));
	// Create new projection matrix.
	m_projectionMatrix = glm::ortho(m_projectionValues[0], m_projectionValues[1], m_projectionValues[2], m_projectionValues[3], m_projectionValues[4], m_projectionValues[5]);
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

glm::vec3 Camera::pixelCoordsToWorldCoords(const glm::vec2& pixelCoords)
{
	glm::vec3 pixelCoords3 = glm::vec3{ pixelCoords.x, m_viewportVec[3] - pixelCoords.y, 0.0f };
	m_viewMatrix = m_scalingMatrix * m_rotationMatrix * m_translationMatrix;
	glm::vec3 worldVec = glm::unProject(pixelCoords3, m_viewMatrix, m_projectionMatrix, m_viewportVec);
	return worldVec;
}

// TODO: This function need to be checked.
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
