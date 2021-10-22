#pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <glm.hpp>
#include "GLM/gtc/quaternion.hpp"
#include "GLM/gtx/quaternion.hpp"

//=============================================================================================================================================//
//  Class.																																	   //
//=============================================================================================================================================//

class Camera 
{
private:

public:

	// Camera placement vectors.
	glm::vec3 m_position;			// The camera position in the world.
	glm::vec3 m_target;				// The position the camera looks at.
	glm::vec3 m_orientation;		// The orientation of the camera.
	glm::vec3 m_rightVector;		// The x-axis (right) of the camera.						
	glm::vec3 m_upVector;			// The topo of the camera.
	float m_targetDistance;			// The distance from the target to the camera.

	// Movement.
	float m_panSpeed = 0.025f;			// Speed at which the camera is panned.
	float m_targetTranslateSpeed = 0.4f;// The rate at which the camera moves towards the target.
	float m_rotationSpeed = 0.005f;		// Rotation speed of the camera.
	float m_lastFrame = 0.0f;			// The time of the previously rendered frame.
	float m_deltaTime = 0.0f;			// The difference between the last 2 rendred frame.
										// This is used to ensure constant movement and sensitivity
										// regardless of the framerate.
	
	// View matrix components.
	//glm::mat4 m_scalingMatrix = glm::mat4(1.0f);		// Handles camera scaling.
	//glm::mat4 m_translationMatrix = glm::mat4(1.0f);	// Handles camera translations.
	//glm::mat4 m_rotationMatrix = glm::mat4(1.0f);		// Handles camera rotations.
	glm::mat4* m_viewMatrix;							// View matrix used by the engine.

	// The quaternion used to calculate rotation.
	//glm::quat m_rotationQuaternion;

	// Contructor.
	Camera(float position[3], float target[3], glm::mat4* viewMatrix);
	// Destructor.
	~Camera();

	// Calculates the delta time and returns it.	
	float deltaTime();

	// Camera controls.
	void translateTowardsTarget(float translation);
	void rotateAroundTarget(float deltaPixels[2]);
	void translateCamera(float deltaPixels[2]);

	void rotateAroundPosition(float rotation[3]);
	void scaleScene(float scale);

	// Update the view matrix that was supplied by the engine.
	void updateView();
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//