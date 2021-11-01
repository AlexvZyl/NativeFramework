//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "CameraGL.h"
#include "GLFW/glfw3.h"
#include "GLM/gtx/rotate_vector.hpp"

//=============================================================================================================================================//
//  Functions.																																   //
//=============================================================================================================================================//

// Contructor.
Camera::Camera(float position[3], float target[3], glm::mat4* viewMatrix)
	: m_viewMatrix(viewMatrix)
{
	// Setup camera vectors.
	m_position = glm::vec3(position[0], position[1], position[2]);
	m_target = glm::vec3(target[0], target[1], target[2]);
	m_orientation = m_position - m_target;
	m_targetDistance = glm::length(m_orientation);
	m_rightVector = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), m_orientation));
	m_upVector = glm::normalize(glm::cross(m_orientation, m_rightVector));

	// Init the view matrix.
	*m_viewMatrix = glm::lookAt(m_position, m_target, m_upVector);
}

// Destructor.
Camera::~Camera() 
{
}

// Moves the camera closer or further away from the target.
void Camera::translateTowardsTarget(float translation) 
{
	// Create a new vector that containts the translation information.
	glm::vec3 translationVec = glm::normalize(m_orientation) * translation * m_targetTranslateSpeed;
	// Check that the camera does not move through the target.
	if (glm::length(translationVec) < glm::length(m_orientation) || translation < 0)
	{
		// Add the translation to the orientation vector.
		m_orientation -= translationVec;
		// Create the camera position based on the new orientation vector.
		m_position = m_target + m_orientation;
	}
	//else 
	//{
	//	// Add the translation to the orientation vector.
	//	m_position -= translationVec;
	//	m_target -= translationVec;
	//	m_orientation = m_position - m_target;		
	//}
}

// Moves the camera based on the world's coordinate system, not the camera.
void Camera::translateWorldCoords(float translateVector[3]) 
{
	float scale = 1000000;
	// Move camera position.
	m_position[0] += translateVector[0] * scale;
	m_position[1] += translateVector[1] * scale;
	m_position[2] += translateVector[2] * scale;
	// Update camera target.
	m_target[0] += translateVector[0] * scale;
	m_target[1] += translateVector[1] * scale;
	m_target[2] += translateVector[2] * scale;
}

// Rotates the camera around the target.
void Camera::rotateAroundTarget(float deltaPixels[2])
{
	// Calculate the axis' of rotation.
	glm::vec3 xRotate = glm::cross(m_orientation, m_upVector - m_orientation);
	glm::vec3 yRotate = glm::cross(m_orientation, m_rightVector - m_orientation);
	// Rotate the orientation vector.
	m_orientation = glm::rotate(m_orientation, deltaPixels[1] * m_rotationSpeed, xRotate);
	m_orientation = glm::rotate(m_orientation, -1* deltaPixels[0] * m_rotationSpeed, yRotate);
	// Check the the camera is not looking straight down or up.
	if ( (glm::normalize(m_orientation)[1] <= 0.99f) && (glm::normalize(m_orientation)[1] >= -0.99f) )
	{
		// Recalculate the camera position.
		m_position = m_target + m_orientation;
		// Update the right and up vector.
		m_rightVector = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), m_orientation));
		m_upVector = glm::normalize(glm::cross(m_orientation, m_rightVector));
	}
	else
	{
		m_orientation = glm::rotate(m_orientation, deltaPixels[0] * m_rotationSpeed, yRotate);
		m_orientation = glm::rotate(m_orientation, deltaPixels[1] * m_rotationSpeed, xRotate);
	}
}

// Translates the camera based on mouse movement.
void Camera::panCamera(float panVector[2]) 
{
	// Calculate the tranlation vectors.
	glm::vec3 xTranslate = glm::normalize(m_rightVector) * panVector[0] * m_panSpeed;
	glm::vec3 yTranslate = glm::normalize(m_upVector) * panVector[1] * m_panSpeed;
	// Apply the translation vectors.
	m_target -= xTranslate - yTranslate;
	m_position -= xTranslate - yTranslate;
}

// Update the view matrix based on the camera variables.
void Camera::updateView()
{
	// Update the egnine view matrix.
	*m_viewMatrix = glm::lookAt(m_position, m_target, m_upVector);
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//