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
#include "CoreGL/VertexArrayObjectGL.h"

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
		m_circuit.m_components.insert(m_circuit.m_components.end(), m_activeComponent);
		designerState = ENTITY_SELECT;
	}
	else if (designerState == ENTITY_SELECT) {
		m_currentEntityID = getEntityID(pixelCoords);
		setActiveComponent(m_currentEntityID);
	}
}

void Design2DEngineGL::mousePressRight(float pixelCoords[2])
{
	// Call base engine event.
	Base2DEngineGL::mousePressRight(pixelCoords);
	// Update current entity ID.
	m_currentEntityID = getEntityID(pixelCoords);
	setActiveComponent(m_currentEntityID);
	// Pass the active component to the GUI state for editing.
	m_guiState->active_component = m_activeComponent.get();
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
		std::cout << m_currentEntityID << std::endl;
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
		p1 = std::make_unique<Circle<VertexDataCircle>>(m_circleEntitiesVAO.get(), v1, 0.25, colour, 0.2f, 0.01f, nullptr);
		p1->update();
	}
	if (key == GLFW_KEY_W && action == GLFW_PRESS) 
	{ 
		p2 = std::make_unique<Circle<VertexDataCircle>>(m_circleEntitiesVAO.get(), v2, 0.25, colour, 0.2f, 0.01f, nullptr);
		p2->update();
	}
	if (key == GLFW_KEY_E && action == GLFW_PRESS) 
	{
		p3 = std::make_unique<Circle<VertexDataCircle>>(m_circleEntitiesVAO.get(), v3, 0.25, colour, 0.2f, 0.01f, nullptr);
		p3->update();
	}

	// Remove components.
	if (key == GLFW_KEY_A && action == GLFW_PRESS) { p1 = nullptr; }
	if (key == GLFW_KEY_S && action == GLFW_PRESS) { p2 = nullptr; }
	if (key == GLFW_KEY_D && action == GLFW_PRESS) { p3 = nullptr; }

	if (action == GLFW_PRESS) {
		float pixelCoords[] = { m_guiState->renderWindowMouseCoordinate.x, m_guiState->renderWindowMouseCoordinate.y };
		glm::vec3 WorldCoords = pixelCoordsToWorldCoords(pixelCoords);
		float screenCoords[2] = { WorldCoords[0], WorldCoords[1] };
		switch (key) {
		case GLFW_KEY_P:
			if (designerState != COMPONENT_PLACE) {
				designerState = COMPONENT_PLACE;
				//add a dummy component
				if (m_activeComponent) {
					m_activeComponent->unhighlight();
				}
				m_activeComponent = std::make_shared<Component2D>(screenCoords,
																  m_triangleEntitiesVAO.get(),
																  m_lineEntitiesVAO.get(),
																  m_triangleTexturedEntitiesVAO.get(),
																  m_circleEntitiesVAO.get(),
																	&m_circuit);
			}
			break;
		case GLFW_KEY_ESCAPE:
			designerState = ENTITY_SELECT;
			//m_activeComponent->destroy();
			//delete m_activeComponent;
			//Remove the dummy component
			m_activeComponent = NULL;//runs deconstructor
			m_circleEntitiesVAO->updateGPU(); // Temporary work-around for disappearing circles.
			break;
		case GLFW_KEY_DELETE:
			if ((designerState = ENTITY_SELECT) && m_activeComponent) {
				auto iterator = std::find(m_circuit.m_components.begin(), m_circuit.m_components.end(), m_activeComponent);
				if (iterator != m_circuit.m_components.end())
				{
					m_circuit.m_components.erase(iterator);
					m_activeComponent = NULL;
					m_circleEntitiesVAO->updateGPU(); //Temporary work-around for disappearing circles.
				}
			}
			break;
		}
	}
}



//Helper functions
void Design2DEngineGL::setActiveComponent(unsigned eID) {

	if (m_activeComponent) {
		m_activeComponent->unhighlight();
	}
	if ((eID == 0) || (eID == -1)) {
			m_activeComponent = NULL;
			m_circleEntitiesVAO->updateGPU(); //Temporary work-around for disappearing circles.
		m_guiState->clickedZone.background = true;
	}
	else {
		m_guiState->clickedZone.background = false;
		ManagedEntity* currentEntity = EntityManager::getEntity(eID);
		currentEntity->setContext(m_guiState);
		while (currentEntity->m_type != EntityType::COMPONENT) {
			currentEntity = currentEntity->m_parent;
		}

		//This cast remains valid provided all entities on screen are decendents of components. If not, this needs to change.
		Component2D* cur = dynamic_cast<Component2D*>(currentEntity);
		//m_activeComponent = dynamic_cast<std::shared_ptr>(cur);
		m_activeComponent = *std::find_if(begin(m_circuit.m_components), end(m_circuit.m_components), [&](std::shared_ptr<Component2D> current)
			{
				return current.get() == cur;
			});
		m_activeComponent->highlight();
	}
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//