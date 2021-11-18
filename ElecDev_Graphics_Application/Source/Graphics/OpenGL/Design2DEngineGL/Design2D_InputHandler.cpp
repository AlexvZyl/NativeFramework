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
#include "CoreGL/Entities/Circle.h"
#include <GLFW/glfw3.h>
#include "CoreGL/Entities/EntityManager.h"
#include "CoreGL/Entities/Circle.h"
#include <iostream>

//=============================================================================================================================================//
//  Press event.																															   //
//=============================================================================================================================================//

void Design2DEngineGL::mousePressLeft(float pixelCoords[2])
{
	// Call base engine event.
	Base2DEngineGL::mousePressLeft(pixelCoords);


	if (designerState == COMPONENT_PLACE)
	{
		glm::vec3 WorldCoords = pixelCoordsToWorldCoords(pixelCoords);
		float screenCoords[2] = { WorldCoords[0], WorldCoords[1] };
		m_activeComponent->place(screenCoords);
		m_components.insert(m_components.end(), m_activeComponent);
		m_activeComponent = NULL;/*std::make_shared<Component2D>(pixelCoords,
			m_triangleEntitiesVAO.get(),
			m_lineEntitiesVAO.get(),
			m_triangleTexturedEntitiesVAO.get(),
			m_circleEntitiesVAO.get());*/
		designerState = ENTITY_SELECT;
	}
	else if (designerState == ENTITY_SELECT) {
		m_currentEntityID = getEntityID(pixelCoords);
		if ((m_currentEntityID == 0) || (m_currentEntityID == -1)) {

		}
		else {
			ManagedEntity* currentEntity = EntityManager::getEntity(m_currentEntityID);
			while (currentEntity->m_parent != nullptr) {
				currentEntity = currentEntity->m_parent;
			}
			Component2D* cur = dynamic_cast<Component2D*>(currentEntity);
			m_activeComponent = std::make_shared<Component2D>(*cur);
		}
	}
}

void Design2DEngineGL::mousePressRight(float pixelCoords[2])
{
	// Call base engine event.
	Base2DEngineGL::mousePressRight(pixelCoords);
	// Update current entity ID.
	m_currentEntityID = getEntityID(pixelCoords);
	if ((m_currentEntityID == 0) || (m_currentEntityID == -1)) {
		m_guiState->clickedZone.background = true;
	}
	else {
		ManagedEntity* currentEntity = EntityManager::getEntity(m_currentEntityID);
		currentEntity->setContext(m_guiState);
	}
}

void Design2DEngineGL::mousePressMiddle(float pixelCoords[2])
{
	// Call base engine event.
	Base2DEngineGL::mousePressMiddle(pixelCoords);
}

//=============================================================================================================================================//
//  Move event.																																   //
//=============================================================================================================================================//

void Design2DEngineGL::mouseMoveEvent(float pixelCoords[2], int buttonStateLeft, int buttonStateRight, int buttonStateMiddle)
{
	if (designerState == COMPONENT_PLACE) {
		// Move the component.
		glm::vec3 WorldCoords = pixelCoordsToWorldCoords(pixelCoords);
		float screenCoords[2] = { WorldCoords[0], WorldCoords[1] };
		m_activeComponent->moveTo(screenCoords);
	}
	m_currentEntityID = getEntityID(pixelCoords);

	#ifdef _DEBUG
		//std::cout << m_currentEntityID << std::endl;
	#endif

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

	/*
	// Move the component.
	glm::vec3 WorldCoords = pixelCoordsToWorldCoords(pixelCoords);
	float screenCoords[2] = { WorldCoords[0], WorldCoords[1] };
	m_activeComponent->moveTo(screenCoords);
	m_activeComponent->draw();*/
}

//=============================================================================================================================================//
//  Key press.																																   //
//=============================================================================================================================================//

void Design2DEngineGL::keyEvent(int key, int action)
{
	glm::vec3 v1(-1.f, -1.f, 0.0f);
	glm::vec3 v2(0.f, 0.f, 0.0f);
	glm::vec3 v3(1.f, 1.f, 0.0f);

	glm::vec4 colour(1.f, 0.f, 0.f, 1.f);

	// Add components.
	if (key == GLFW_KEY_Q && action == GLFW_PRESS) 
	{ 
		p1 = new Circle<VertexDataCircle>(m_circleEntitiesVAO.get(), v1, 0.25, colour, 0.2f, 0.0f, nullptr);
		p1->update();
	}
	if (key == GLFW_KEY_W && action == GLFW_PRESS) 
	{ 
		p2 = new Circle<VertexDataCircle>(m_circleEntitiesVAO.get(), v2, 0.25, colour, 0.2f, 0.0f, nullptr);
		p2->update();
	}
	if (key == GLFW_KEY_E && action == GLFW_PRESS) 
	{
		p3 = new Circle<VertexDataCircle>(m_circleEntitiesVAO.get(), v3, 0.25, colour, 0.2f, 0.0f, nullptr);
		p3->update();
	}

	// Remove components.
	if (key == GLFW_KEY_A && action == GLFW_PRESS) { p1->destroy(); }
	if (key == GLFW_KEY_S && action == GLFW_PRESS) { circ2->destroy(); }
	if (key == GLFW_KEY_D && action == GLFW_PRESS) { circ3 = nullptr; }*/

	if (action == GLFW_PRESS) {
		float pixelCoords[] = { m_guiState->renderWindowMouseCoordinate.x, m_guiState->renderWindowMouseCoordinate.y };
		glm::vec3 WorldCoords = pixelCoordsToWorldCoords(pixelCoords);
		float screenCoords[2] = { WorldCoords[0], WorldCoords[1] };
		switch (key) {
		case GLFW_KEY_P:
			designerState = COMPONENT_PLACE;
			//add a dummy component
			m_activeComponent = std::make_shared<Component2D>(screenCoords,
				m_triangleEntitiesVAO.get(),
				m_lineEntitiesVAO.get(),
				m_triangleTexturedEntitiesVAO.get(),
				m_circleEntitiesVAO.get());
			break;
		case GLFW_KEY_ESCAPE:
			designerState = ENTITY_SELECT;
			//m_activeComponent->destroy();
			//Remove the dummy component
			m_activeComponent = NULL;//runs deconstructor
			for (int i = 0; i < m_components.size(); i++) {
				m_components[i]->update();
			}
			break;
		case GLFW_KEY_DELETE:
			if ((designerState = ENTITY_SELECT) && m_activeComponent != NULL) {
				m_activeComponent->destroy();
			}
			break;
		}
	}
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//