//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Camera.h"
#include <ShaderHandler/ShaderHandler.h>
#include "GLFW/glfw3.h"

// GLM Functions.
#include "glm.hpp"
#include "GLM/gtc/quaternion.hpp"
#include "GLM/gtx/quaternion.hpp"


//=============================================================================================================================================//
//  Functions.																																   //
//=============================================================================================================================================//

// Contructor.
Camera::Camera(float position[3], float target[3], glm::mat4* rotationMatrix, glm::mat4* translationMatrix)
	: m_rotationMatrix(rotationMatrix), m_translationMatrix(translationMatrix)
{
	// Init the variables.
	m_position = glm::vec3(position[0], position[1], position[2]);
	m_target = glm::vec3(target[0], target[1], target[2]);
}

// Destructor.
Camera::~Camera() 
{
	
}

// Move the camera.
void Camera::translationEvent(float translation[3]) 
{
	
}

// Rotate the camera.
void Camera::rotationEvent(float rotation[3])
{
	
}

// Update the view matrix based on the camera variables.
void Camera::updateMatrices()
{
	// Convert the rotation quaternion to a matrix.
	//m_rotationMatrix = glm::toMat4(m_rotationQuaternion);
}

// Calculates the delta time and returns it.
// Another use case can be to use this function to update the
// delta time and just use the internal variable m_deltaTime.
float Camera::deltaTime() 
{
	float currentFrame = glfwGetTime();
	m_deltaTime = currentFrame - m_lastFrame;
	m_lastFrame = currentFrame;
	return m_deltaTime;
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//