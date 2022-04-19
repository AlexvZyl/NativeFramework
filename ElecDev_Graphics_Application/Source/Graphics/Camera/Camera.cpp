//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"

//==============================================================================================================================================//
//  Functions.																																	//
//==============================================================================================================================================//

Camera::Camera(CameraType cameraType, const glm::vec2& size)
	: m_type(cameraType)
{
	setViewport(size);

	// Create the camera.
	switch (cameraType) 
	{
	case CameraType::Standard2D:
		construct2DCamera(size);
		break;

	case CameraType::Standard3D:
		construct3DCamera(size);
		break;
	}
}

void Camera::construct2DCamera(const glm::vec2& size)
{
	// Find the minimum value of the viewport dimensions.
	float minValue = 0.f;
	if (m_viewportVec[2] < m_viewportVec[3])
	{
		minValue = m_viewportVec[2];
	}
	else
	{
		minValue = m_viewportVec[3];
	}

	// Scale the projection values according to the viewport.
	m_projectionValues = { -m_viewportVec[2] / minValue, m_viewportVec[2] / minValue,
						   -m_viewportVec[3] / minValue, m_viewportVec[3] / minValue,
						   -1.0,						 1.0 };

	// Create projection matrix.
	m_projectionMatrix = glm::ortho(m_projectionValues[0], m_projectionValues[1], 
									m_projectionValues[2], m_projectionValues[3], 
									m_projectionValues[4], m_projectionValues[5]);
}

void Camera::construct3DCamera(const glm::vec2& size)
{
	// TODO.
}

void Camera::onUpdate() 
{
	if (m_viewMatrixUpToDate) return;

	m_viewMatrix = m_scalingMatrix * m_rotationMatrix * m_translationMatrix; 
	m_viewMatrixUpToDate = true;
}

void Camera::resize(const glm::vec2& size) 
{
	setViewport(size);
	// Scale projection values.
	glm::vec2 scalingFactor = { size.x / m_viewportVec[2], size.y / m_viewportVec[3]};
	m_projectionValues = { m_projectionValues[0] * scalingFactor.x, m_projectionValues[1] * scalingFactor.x,
						   m_projectionValues[2] * scalingFactor.y, m_projectionValues[3] * scalingFactor.y,
						   m_projectionValues[4],				    m_projectionValues[5] };

	// Scale with the y scaling.
	//m_scalingMatrix = glm::scale(m_scalingMatrix, glm::vec3(scalingFactor[1], scalingFactor[1], 1.0f));
	
	// Update base matrix.
	m_scalingMatrixHome = glm::scale(m_scalingMatrixHome, glm::vec3(scalingFactor[1], scalingFactor[1], 1.0f));

	// Create new projection matrix.
	m_projectionMatrix = glm::ortho(m_projectionValues[0], m_projectionValues[1], 
									m_projectionValues[2], m_projectionValues[3], 
									m_projectionValues[4], m_projectionValues[5]);
}

const CameraType& Camera::getType() const
{
	return m_type;
}

//==============================================================================================================================================//
//  Controls.																																	//
//==============================================================================================================================================//

void Camera::translate(const glm::vec2& translation) 
{
	translate({ translation.x, translation.y, 0.f });
}

void Camera::translate(const glm::vec3& translation) 
{
	m_translationMatrix = glm::translate(m_translationMatrix, translation);
	moved();
}

void Camera::moved() 
{
	m_viewMatrixUpToDate = false;
}

//==============================================================================================================================================//
//  Coordinate systems.																															//
//==============================================================================================================================================//

const glm::vec4& Camera::getViewport() const 
{
	return m_viewportVec;
}

const glm::vec2& Camera::getViewportSize() const
{
	return { m_viewportVec[2] - m_viewportVec[0],
			 m_viewportVec[3] - m_viewportVec[1] };
}

void Camera::setViewport(const glm::vec2& viewport) 
{
	m_viewportVec = { 0.f, 0.f, viewport.x, viewport.y };
}

void Camera::setViewport(const glm::vec4& viewport)
{
	m_viewportVec = viewport;
}

// This will most likely not work with 3D, need to calculate the z value.
glm::vec3 Camera::pixelCoordsToWorldCoords(const glm::vec2& pixelCoords)
{
	if (!m_viewMatrixUpToDate)
	{
		m_viewMatrix = m_scalingMatrix * m_rotationMatrix * m_translationMatrix;
		m_viewMatrixUpToDate = true;
	}
	// GLFW passes (0,0) as top left.  Lumen has (0,0) has bottom left.
	return glm::unProject({ pixelCoords.x, m_viewportVec[3] - pixelCoords.y, 0.0f }, 
						    m_viewMatrix, m_projectionMatrix, m_viewportVec);
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
	if (!m_viewMatrixUpToDate)
	{
		m_viewMatrix = m_scalingMatrix * m_rotationMatrix * m_translationMatrix;
		m_viewMatrixUpToDate = true;
	}
	return screenVec * glm::inverse(m_viewMatrix * m_projectionMatrix);
}

const glm::mat4& Camera::getViewMatrix() const 
{
	return m_viewMatrix;
}

const glm::mat4& Camera::getProjectionMatrix() const 
{
	return m_projectionMatrix;
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//