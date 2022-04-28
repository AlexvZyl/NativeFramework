//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Utilities/Logger/Logger.h"

//==============================================================================================================================================//
//  Functions.																																	//
//==============================================================================================================================================//

Camera::Camera(CameraType cameraType, const glm::vec2& size)
	: m_type(cameraType)
{
	float aspectRatio = size.x / size.y;

	switch (cameraType) 
	{
	case CameraType::Standard2D:
		m_projectionMatrix = glm::ortho(-aspectRatio, aspectRatio, -1.f, 1.f);
		break;

	case CameraType::Standard3D:
		break;
	}

	projectionChanged();
	setViewport(size);
	setScaleRate(1.f);
	updateAllMatrices();
}

void Camera::resize(const glm::vec2& size) 
{
	float aspectRatio = size.x / size.y;
	
	switch (m_type) 
	{
	case CameraType::Standard2D:
		m_projectionMatrix = glm::ortho(-aspectRatio, aspectRatio, -1.f, 1.f);
		break;

	case CameraType::Standard3D:
		break;
	}
	
	setViewport(size);
	projectionChanged();
}

void Camera::resize(const glm::vec4& viewport)
{
	float aspectRatio = (viewport[2] - viewport[0]) / (viewport[3] - viewport[1]);
	
	switch (m_type) 
	{
	case CameraType::Standard2D:
		m_projectionMatrix = glm::ortho(-aspectRatio, aspectRatio, -1.f, 1.f);
		break;

	case CameraType::Standard3D:
		break;
	}
	
	setViewport(viewport);
	projectionChanged();
}

const CameraType& Camera::getType() const
{
	return m_type;
}

void Camera::onUpdate()
{
	m_prevViewMatrix = m_viewMatrix;
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
	Camera::scale({ scale, scale, 1.f });
}

void Camera::scale(const glm::vec3& scale) 
{
	m_scalingMatrix = glm::scale(m_scalingMatrix, scale);
	viewChanged();
}

void Camera::scaleAroundCursor(float scale, const glm::vec2& cursor)
{
	if (m_type == CameraType::Standard2D)
	{
		glm::vec2 coordsBeforeScaling = pixelToWorldCoords(cursor);
		Camera::scale({ scale, scale, 1.f });
		glm::vec2 coordsAfterScaling = pixelToWorldCoords(cursor, true);
		Camera::translate({ coordsAfterScaling.x - coordsBeforeScaling.x, coordsAfterScaling.y - coordsBeforeScaling.y, 0.f });
	}
	else 
	{
		LUMEN_LOG_WARN("Unknown type.", "Camera");
	}
}

void Camera::incrementZoomLevel(int increment)
{
	if (m_type == CameraType::Standard2D)
	{
		float scale = getScaleFromIncrement(increment);
		Camera::scale({ scale, scale, 1.f });
	}
	else
	{
		LUMEN_LOG_WARN("Unknown type.", "Camera");
	}
}

void Camera::incrementZoomAroundCursor(int increment, const glm::vec2& cursor)
{
	if (m_type == CameraType::Standard2D)
	{
		glm::vec2 coordsBeforeScaling = pixelToWorldCoords(cursor, true);
		float scale = getScaleFromIncrement(increment);
		Camera::scale({ scale, scale, 1.f });
		glm::vec2 coordsAfterScaling = pixelToWorldCoords(cursor, true);
		Camera::translate({ coordsAfterScaling.x - coordsBeforeScaling.x, coordsAfterScaling.y - coordsBeforeScaling.y, 0.f });
	}
	else
	{
		LUMEN_LOG_WARN("Unknown type.", "Camera");
	}
}

float Camera::getScaleFromIncrement(int increment)
{
	if (increment >= 0)
	{
		return 1 + (m_scaleRate * increment);
	}
	else
	{
		return 1 / (1 + m_scaleRate * -1 * increment);
	}
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
glm::vec3 Camera::pixelToWorldCoords(const glm::vec2& pixelCoords, bool useUpdatedView)
{
	if (useUpdatedView)
	{
		updateViewMatrix();
		return glm::unProject({ pixelCoords.x, pixelCoords.y, 0.0f },
								m_viewMatrix, m_projectionMatrix, m_viewport);
	}
	else 
	{
		return glm::unProject({ pixelCoords.x, pixelCoords.y, 0.0f },
								m_prevViewMatrix, m_projectionMatrix, m_viewport);
	}
}

glm::vec2 Camera::worldToPixelCoords(const glm::vec3& worldCoords, bool useUpdatedView)
{
	if (useUpdatedView)
	{
		updateViewMatrix();
		return glm::project(worldCoords, m_viewMatrix, m_projectionMatrix, m_viewport);
	}
	else 
	{
		return glm::project(worldCoords, m_prevViewMatrix, m_projectionMatrix, m_viewport);
	}
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
