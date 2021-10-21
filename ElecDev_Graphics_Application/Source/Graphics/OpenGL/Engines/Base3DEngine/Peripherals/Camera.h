#pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <glm.hpp>

//=============================================================================================================================================//
//  Class.																																	   //
//=============================================================================================================================================//

class Camera 
{
private:

public:

	// Camera placement vectors.
	glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 3.0f);					// The camera position in the world.
	glm::vec3 m_target = glm::vec3(0.0f, 0.0f, 0.0f);					// The position the camera looks at.
	glm::vec3 m_orientation = m_position - m_target;					// The orientation of the camera.
	glm::vec3 m_rightVector = glm::normalize(							// The x-axis (right) of the camera.
								glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), m_orientation));								
	glm::vec3 m_upVector = glm::cross(m_orientation, m_rightVector);	// The topo of the camera.

	// Movement.
	float m_speed = 0.1f;			// Movement speed of the camera in the world.
	float m_sensitivity = 100.0f;	// Rotation sensitivity of the camera.
	float m_lastFrame = 0.0f;		// The time of the previously rendered frame.
	float m_deltaTime = 0.0f;		// The difference between the last 2 rendred frame.
									// This is used to ensure constant movement and sensitivity
									// regardless of the framerate.

	// The rotation matrix of the engine.  The camera class will take control of
	// it and make calculations.
	glm::mat4* m_rotationMatrix;
	glm::mat4* m_translationMatrix;

	// The quaternion used to calculate rotation.
	glm::quat m_rotationQuaternion;

	// Contructor.
	Camera(float position[3], float target[3], glm::mat4* rotationMatrix, glm::mat4* translationMatrix);
	// Destructor.
	~Camera();

	// Calculates the delta time and returns it.	
	float deltaTime();

	// Camera controls.
	void translationEvent(float translation[3]);
	void rotationEvent(float rotation[3]);

	// Update the view matrix.
	void updateMatrices();
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//