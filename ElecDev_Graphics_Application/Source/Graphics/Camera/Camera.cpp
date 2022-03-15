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

void Camera::onUpdate() 
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
	// GLFW passes (0,0) as top left.  Lumen has (0,0) has bottom left.
	glm::vec3 pixelCoords3 = glm::vec3{ pixelCoords.x, m_viewportVec[3] - pixelCoords.y, 0.0f };
	m_viewMatrix = m_scalingMatrix * m_rotationMatrix * m_translationMatrix;  // Do we want this to be updated?
	return glm::unProject(pixelCoords3, m_viewMatrix, m_projectionMatrix, m_viewportVec);
}

// TODO: This function needs to be checked.
glm::vec3 Camera::pixelCoordsToCameraCoords(const glm::vec2& pixelCoords)
{
	glm::vec2 viewport = { m_viewportVec[2], m_viewportVec[3]};
	// GLFW passes (0,0) as top left.  Lumen has (0,0) has bottom left.
	glm::vec2 pixelCoordsTemp = { pixelCoords[0] , (float)viewport[1] - pixelCoords[1] };
	glm::vec2 normalizedScreenCoords = {
		(pixelCoordsTemp.x - viewport.x / 2) / (viewport.x / 2),
		(pixelCoordsTemp.y - viewport.y / 2) / (viewport.y / 2)
	};
	glm::vec4 screenVec = { normalizedScreenCoords.x, normalizedScreenCoords.y, 0.0f, 1.0f };
	m_viewMatrix = m_scalingMatrix * m_rotationMatrix * m_translationMatrix;  // Do we want this to be updated?
	return screenVec * glm::inverse(m_modelMatrix * m_viewMatrix * m_projectionMatrix);
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//