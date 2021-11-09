/*
This is where the drawing enigine mouse events are handled.
*/

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Design2D_Engine.h"
// Entities.
#include "Peripherals/Component2D.h"
#include "CoreGL/Entities/Polygon.h"
#include <GLFW/glfw3.h>

//=============================================================================================================================================//
//  Press event.																															   //
//=============================================================================================================================================//

// Event handler for a mouse left click.
void Design2DEngineGL::mousePressLeft(float pixelCoords[2])
{
	// Call base engine event.
	Base2DEngineGL::mousePressLeft(pixelCoords);

	m_activeComponent = std::make_shared<Component2D>(pixelCoords);
	m_components.insert(m_components.end(), m_activeComponent);
}

// Event handler for a mouse right click.
void Design2DEngineGL::mousePressRight(float pixelCoords[2])
{
	// Call base engine event.
	Base2DEngineGL::mousePressRight(pixelCoords);
}

//=============================================================================================================================================//
//  Move event.																																   //
//=============================================================================================================================================//

// Event handler for a mouse move event.
void Design2DEngineGL::mouseMoveEvent(float pixelCoords[2], int buttonStateLeft, int buttonStateRight, int buttonStateMiddle)
{
	// Move the component.
	glm::vec3 WorldCoords = pixelCoordsToWorldCoords(pixelCoords);
	float screenCoords[2] = { WorldCoords[0], WorldCoords[1] };
	m_activeComponent->moveTo(screenCoords);
	m_activeComponent->draw();

	// Call parent event.
	Base2DEngineGL::mouseMoveEvent(pixelCoords, buttonStateLeft, buttonStateRight, buttonStateMiddle);
}

//=============================================================================================================================================//
//  Scroll event.																															   //
//=============================================================================================================================================//

void Design2DEngineGL::mouseScrollEvent(float pixelCoords[2], float yOffset)
{
	// Call the base engine event.
	Base2DEngineGL::mouseScrollEvent(pixelCoords, yOffset);

	// Move the component.
	glm::vec3 WorldCoords = pixelCoordsToWorldCoords(pixelCoords);
	float screenCoords[2] = { WorldCoords[0], WorldCoords[1] };
	m_activeComponent->moveTo(screenCoords);
	m_activeComponent->draw();
}

//=============================================================================================================================================//
//  Key press.																																   //
//=============================================================================================================================================//

void Design2DEngineGL::keyEvent(int key, int action) 
{
	glm::vec3 v1(-0.5f, 0.5f, 0.0f);
	glm::vec3 v2(0.5f, 0.5f, 0.0f);
	glm::vec3 v3(0.5f, -0.5f, 0.0f);
	glm::vec3 v4(-0.5f, -0.5f, 0.0f);
	std::vector<glm::vec3> vertices1 = { v1, v2, v3, v4 };
	glm::vec3 v5(-0.5f + 1, 0.5f + 1, 0.0f);
	glm::vec3 v6(0.5f + 1, 0.5f + 1, 0.0f);
	glm::vec3 v7(0.5f + 1, -0.5f + 1, 0.0f);
	glm::vec3 v8(-0.5f + 1, -0.5f + 1, 0.0f);
	std::vector<glm::vec3> vertices2 = { v5, v6, v7, v8 };
	glm::vec3 v9(-0.5f-1, 0.5f-1, 0.0f);
	glm::vec3 v10(0.5f-1, 0.5f-1, 0.0f);
	glm::vec3 v11(0.5f-1, -0.5f-1, 0.0f);
	glm::vec3 v12(-0.5f-1, -0.5f-1, 0.0f);
	std::vector<glm::vec3> vertices3 = { v9, v10, v11, v12 };

	// Add components.
	if (key == GLFW_KEY_Q && action == GLFW_PRESS) { p1 = new Polygon2D(vertices3, m_trianglesVAO); }
	if (key == GLFW_KEY_W && action == GLFW_PRESS) { p2 = new Polygon2D(vertices1, m_trianglesVAO); }
	if (key == GLFW_KEY_E && action == GLFW_PRESS) { p3 = new Polygon2D(vertices2, m_trianglesVAO); }

	// Remove components.
	if (key == GLFW_KEY_A && action == GLFW_PRESS) { p1->destroy(); }
	if (key == GLFW_KEY_S && action == GLFW_PRESS) { p2->destroy(); }
	if (key == GLFW_KEY_D && action == GLFW_PRESS) { p3->destroy(); }
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//