#pragma once

/* 
This code contains the class that handles the mouse point that snaps to components on the screen.
*/

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include <glad/glad.h>
#include "CoreGL/VertexArrayObjectGL.h"
#include <GLFW/glfw3.h>
#include "CoreGL/ShaderHandlerGL.h"
#include "stateMachine.h"

//----------------------------------------------------------------------------------------------------------------------
//  Mouse point class.
//----------------------------------------------------------------------------------------------------------------------

class MousePoint 
{

private:

	// VAO that contains the mouse point.
	VertexArrayObject* m_VAO;
	// Shader.
	Shader* m_shader;
	// Access to the state mafhine.
	stateMachine* m_states;

	// View matrix components.
	glm::mat4 m_scalingMatrix = glm::mat4(1.0f);	// Handles camera scaling.
	glm::mat4 m_translationMatrix = glm::mat4(1.0f);// Handles camera translations.
	glm::mat4 m_rotationMatrix = glm::mat4(1.0f);	// Handles camera rotations.

	// MVP Matrices.
	glm::mat4 m_modelMatrix = glm::mat4(1.0f);		// This is going to stay an identiy matrix.
	glm::mat4 m_viewMatrix = glm::mat4(1.0f);		// Changes with matrix components.
	glm::mat4* m_projectionMatrix;					// This is a pointer to the DesignEngine's matrix so that it scales
													// accordingly.

public:

	// Mouse pixel coordinates.
	float m_worldCoordsPrev[2] = {0,0};

	//Constructor
	MousePoint(float color[4], float radius, unsigned int resolution, glm::mat4* projectionMatrix, stateMachine* states);
	// Destructor.
	~MousePoint();

	// Update the mouse point and render to the screen.
	void render();

	// Update the projection matrix.
	void updateProjection();

	// Update the position of the drawn point.
	void updatePosition(float pixelCoords[2]);

	// The mouse point requires its own conversion since it lives in a different space than that of the drawing.
	glm::vec4 pixelCoordsToWorldCoords(float pixelCoords[2]);
};

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------