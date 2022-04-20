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

	updateAllMatrices();
	setScaleRate(1.f);
}

void Camera::construct2DCamera(const glm::vec2& size)
{
	// Find the minimum value of the viewport dimensions.
	float minValue = 0.f;
	if (m_viewport[2] < m_viewport[3]) { minValue = m_viewport[2]; }
	else							   { minValue = m_viewport[3]; }

	// Scale the projection values according to the viewport.
	m_projectionValues = { -m_viewport[2] / minValue, m_viewport[2] / minValue,
						   -m_viewport[3] / minValue, m_viewport[3] / minValue,
						   -1.0,						 1.0 };

	// Create projection matrix.
	m_projectionMatrix = glm::ortho(m_projectionValues[0], m_projectionValues[1], 
									m_projectionValues[2], m_projectionValues[3], 
									m_projectionValues[4], m_projectionValues[5]);
	projectionChanged();
}

void Camera::construct3DCamera(const glm::vec2& size)
{
	// TODO.
}
void Camera::resize(const glm::vec2& size) 
{
	// Scale projection values.
	glm::vec2 scalingFactor = { size.x / m_viewport[2], size.y / m_viewport[3]};
	setViewport(size);
	m_projectionValues = { m_projectionValues[0] * scalingFactor.x, m_projectionValues[1] * scalingFactor.x,
						   m_projectionValues[2] * scalingFactor.y, m_projectionValues[3] * scalingFactor.y,
						   m_projectionValues[4],				    m_projectionValues[5] };
	
	// Create new projection matrix.
	m_projectionMatrix = glm::ortho(m_projectionValues[0], m_projectionValues[1], 
									m_projectionValues[2], m_projectionValues[3], 
									m_projectionValues[4], m_projectionValues[5]);
}

const CameraType& Camera::getType() const
{
	return m_type;
}

void Camera::onUpdate()
{
	updateAllMatrices();
}

//==============================================================================================================================================//
//  Controls.																																	//
//==============================================================================================================================================//

void Camera::setPosition(const glm::vec3& position)
{
	m_position = position;
	viewChanged();
}

void Camera::setScaleRate(float rate)
{
	m_scaleRate = rate;
	m_zoomInRate = 1 + m_scaleRate;
	m_zoomOutRate = 1 / (1 + m_scaleRate);
}

void Camera::translate(const glm::vec2& translation) 
{
	translate({ translation.x, translation.y, 0.f });
}

void Camera::translate(const glm::vec3& translation) 
{
	m_position += translation;
	viewChanged();
}

void Camera::scale2D(float scale)
{
	m_scalingMatrix = glm::scale(m_scalingMatrix, { scale, scale, 1.f });
	viewChanged();
}

void Camera::scaleAroundCursor2D(float scale, const glm::vec2& cursor)
{
	glm::vec2 coordsBeforeScaling = pixelCoordsToWorldCoords(cursor);
	m_scalingMatrix = glm::scale(m_scalingMatrix, {scale, scale, 1.f});
	viewChanged();
	glm::vec2 coordsAfterScaling = pixelCoordsToWorldCoords(cursor);
	m_position += glm::vec3( coordsAfterScaling.x - coordsBeforeScaling.x, coordsAfterScaling.y - coordsBeforeScaling.y, 0.f );
	viewChanged();
}

void Camera::incrementZoomLevel2D(int increment)
{
	float scale = increment * m_scaleRate;
	m_scalingMatrix = glm::scale(m_scalingMatrix, { scale, scale, 1.f });
	viewChanged();
}

void Camera::incrementZoomAroundCursor2D(int increment, const glm::vec2& cursor)
{
	glm::vec2 coordsBeforeScaling = pixelCoordsToWorldCoords(cursor);
	float scale = 1.f;
	if (increment >= 0)	scale = increment * m_zoomInRate;
	else				scale = -1 * increment * m_zoomOutRate;
	m_scalingMatrix = glm::scale(m_scalingMatrix, { scale, scale, 1.f });
	viewChanged();
	glm::vec2 coordsAfterScaling = pixelCoordsToWorldCoords(cursor);
	m_position += glm::vec3(coordsAfterScaling.x - coordsBeforeScaling.x, coordsAfterScaling.y - coordsBeforeScaling.y, 0.f);
	viewChanged();
}

//==============================================================================================================================================//
//  Matrices.																																	//
//==============================================================================================================================================//

void Camera::viewChanged() 
{
	m_viewMatrixChanged = true;
	m_viewProjectionMatrixChanged = true;
}

void Camera::projectionChanged()
{
	m_projectionMatrixChanged = true;
	m_viewProjectionMatrixChanged = true;
}

void Camera::updateViewMatrix()
{
	if (!m_viewMatrixChanged) return;

	m_viewMatrix = m_scalingMatrix * m_rotationMatrix * glm::translate(glm::mat4(1.f), m_position);
	m_viewMatrixChanged = false;
}

void Camera::updateProjectionMatrix()
{
	if (!m_projectionMatrixChanged) return;

	m_projectionMatrixChanged = false;
}

void Camera::updateViewProjectionMatrix()
{
	if (!m_viewProjectionMatrixChanged) return;

	m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	m_viewProjectionMatrixChanged = false;
}

void Camera::updateAllMatrices() 
{
	updateViewMatrix();
	updateProjectionMatrix();
	updateViewProjectionMatrix();
}

void Camera::setViewport(const glm::vec2& viewport)
{
	m_viewport = { 0.f, 0.f, viewport.x, viewport.y };
}

void Camera::setViewport(const glm::vec4& viewport)
{
	m_viewport = viewport;
}

const glm::vec4& Camera::getViewport() const
{
	return m_viewport;
}

const glm::vec2& Camera::getViewportSize() const
{
	return { m_viewport[2] - m_viewport[0],
			 m_viewport[3] - m_viewport[1] };
}

const glm::mat4& Camera::getViewMatrix() const
{
	return m_viewMatrix;
}

const glm::mat4& Camera::getProjectionMatrix() const
{
	return m_projectionMatrix;
}

const glm::mat4& Camera::getViewProjectionMatrix() const
{
	return m_viewProjectionMatrix;
}

//==============================================================================================================================================//
//  Coordinate systems.																															//
//==============================================================================================================================================//

// This will most likely not work with 3D, need to calculate the z value.
glm::vec3 Camera::pixelCoordsToWorldCoords(const glm::vec2& pixelCoords)
{
	if (m_viewMatrixChanged) updateViewMatrix();

	// GLFW passes (0,0) as top left.  Lumen has (0,0) has bottom left.
	return glm::unProject({ pixelCoords.x, m_viewport[3] - pixelCoords.y, 0.0f }, 
						    m_viewMatrix, m_projectionMatrix, m_viewport);
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//