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
#include "Peripherals/Circuit.h"

// Serialisation.
#include "Utilities/Serialisation/Serialiser.h"
#include "CoreGL/Entities/Text.h"

//=============================================================================================================================================//
//  Press event.																															   //
//=============================================================================================================================================//

void Design2DEngineGL::mousePressLeft(float pixelCoords[2])
{
	// Call base engine event.
	Base2DEngineGL::mousePressLeft(pixelCoords);

	if (designerState == COMPONENT_PLACE) {
		glm::vec3 WorldCoords = pixelCoordsToWorldCoords(pixelCoords);
		float screenCoords[2] = { WorldCoords[0], WorldCoords[1] };
		m_activeComponent->place(screenCoords);
		m_circuit->m_components.insert(m_circuit->m_components.end(), m_activeComponent);
		designerState = ENTITY_SELECT;
	}
	else if (designerState == ENTITY_SELECT) {
		m_currentEntityID = getEntityID(pixelCoords);
		setActiveComponent(m_currentEntityID);
	}
	else if (designerState == CABLE_PLACE) {
		m_currentEntityID = getEntityID(pixelCoords);
		Port* clickedPort = getPort(m_currentEntityID);
		if (!m_activeCable.get()) {
			if (clickedPort == nullptr) {
				return;
			}
			m_activeCable = std::make_shared<Cable>(clickedPort, m_triangleEntitiesVAO.get(), m_circleEntitiesVAO.get(), m_circuit.get());

		}
		else {
			if (clickedPort != nullptr) {
				m_activeCable->attach(clickedPort);
				m_circuit->m_cables.push_back(m_activeCable);
				m_activeCable = nullptr;
			}
			else {
				glm::vec3 WorldCoords = pixelCoordsToWorldCoords(pixelCoords);
				glm::vec2 screenCoords = { WorldCoords.x, WorldCoords.y };
				m_activeCable->addSegment(screenCoords);
			}
		}
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
	else if (designerState == CABLE_PLACE) {
		m_currentEntityID = getEntityID(pixelCoords);
		if (m_activeCable.get()) {
			glm::vec3 WorldCoords = pixelCoordsToWorldCoords(pixelCoords);
			glm::vec2 screenCoords = { WorldCoords.x, WorldCoords.y };
			m_activeCable->extendSegment(screenCoords);
		}
	}
	else if (designerState == ENTITY_SELECT && buttonStateLeft) {
		//Here we are dragging a component
		if (m_activeComponent.get()) {
			// Move the component.
			glm::vec3 WorldCoords = pixelCoordsToWorldCoords(pixelCoords);
			float screenCoords[2] = { WorldCoords[0], WorldCoords[1] };
			m_activeComponent->moveTo(screenCoords);
		}
	}

	#ifdef _DEBUG
		m_currentEntityID = getEntityID(pixelCoords);
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
	}
	if (key == GLFW_KEY_W && action == GLFW_PRESS) 
	{ 
		p2 = std::make_unique<Circle<VertexDataCircle>>(m_circleEntitiesVAO.get(), v2, 0.25, colour, 0.2f, 0.01f, nullptr);
	}
	if (key == GLFW_KEY_E && action == GLFW_PRESS) 
	{
		p3 = std::make_unique<Circle<VertexDataCircle>>(m_circleEntitiesVAO.get(), v3, 0.25, colour, 0.2f, 0.01f, nullptr);
	}

	// Remove components.
	if (key == GLFW_KEY_A && action == GLFW_PRESS) { p1 = nullptr; }
	if (key == GLFW_KEY_S && action == GLFW_PRESS) { p2 = nullptr; }
	if (key == GLFW_KEY_D && action == GLFW_PRESS) { p3 = nullptr; }

	if (action == GLFW_PRESS) {
		float pixelCoords[] = { m_guiState->renderWindowMouseCoordinate.x, m_guiState->renderWindowMouseCoordinate.y };
		glm::vec3 WorldCoords = pixelCoordsToWorldCoords(pixelCoords);
		float screenCoords[2] = { WorldCoords[0], WorldCoords[1] };
		switch (key) 
		{
		// --------------------------------------------------------------------------------------------------------------- //
		case GLFW_KEY_P:
			//Enter component placement mode.
			ComponentPlaceMode(screenCoords);
			m_activeCable = nullptr;
			break;
		// --------------------------------------------------------------------------------------------------------------- //
		case GLFW_KEY_C:
			//enter cable placement mode.
			designerState = CABLE_PLACE;
			m_activeComponent = nullptr;
			break;
		case GLFW_KEY_ESCAPE:
			designerState = ENTITY_SELECT;
			//Remove the dummy component
			m_activeComponent = NULL; // Runs deconstructor.
			m_activeCable = nullptr;
			break;
		// --------------------------------------------------------------------------------------------------------------- //
		case GLFW_KEY_DELETE:
			if ((designerState = ENTITY_SELECT) && m_activeComponent) {
				auto iterator = std::find(m_circuit->m_components.begin(), m_circuit->m_components.end(), m_activeComponent);
				if (iterator != m_circuit->m_components.end())
				{
					m_circuit->m_components.erase(iterator);
					m_activeComponent = NULL;
				}
			}
			break;
		// --------------------------------------------------------------------------------------------------------------- //
		case GLFW_KEY_Z:
			saveToYAML(m_circuit, "Testing");
			break;
		// --------------------------------------------------------------------------------------------------------------- //
		case GLFW_KEY_T:
			m_circuit->m_components.back()->title->updateText("Changed");
			break;
		// --------------------------------------------------------------------------------------------------------------- //
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
		m_guiState->clickedZone.background = true;
	}
	else {
		m_guiState->clickedZone.background = false;
		Entity* currentEntity = EntityManager::getEntity(eID);
		currentEntity->setContext(m_guiState);
		while (currentEntity->m_type != EntityType::COMPONENT) {
			currentEntity = currentEntity->m_parent;
			if (currentEntity->m_parent == nullptr) {
				break;
			}
		}

		//This cast remains valid provided all entities on screen are decendents of components. If not, this needs to change.
		Component2D* cur = dynamic_cast<Component2D*>(currentEntity);
		//m_activeComponent = dynamic_cast<std::shared_ptr>(cur);
		m_activeComponent = *std::find_if(begin(m_circuit->m_components), end(m_circuit->m_components), [&](std::shared_ptr<Component2D> current)
			{
				return current.get() == cur;
			});
		m_activeComponent->highlight();
	}
}

Port* Design2DEngineGL::getPort(unsigned eID) {
	if ((eID == 0) || (eID == -1)) {
		return nullptr;
		m_guiState->clickedZone.background = true;
	}
	else {
		m_guiState->clickedZone.background = false;
		Entity* currentEntity = EntityManager::getEntity(eID);
		currentEntity->setContext(m_guiState);
		while (currentEntity->m_type != EntityType::PORT) {
			currentEntity = currentEntity->m_parent;			
			if (currentEntity->m_parent == nullptr) {
				return nullptr;
			}
		}

		//This cast remains valid provided all entities on screen are decendents of components. If not, this needs to change.
		Port* cur = dynamic_cast<Port*>(currentEntity);
		return cur;
	}
}
//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//