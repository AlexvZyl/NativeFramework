/*
This is where the drawing enigine mouse events are handled.
*/

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Design2D_Engine.h"
#include "OpenGL/SceneGL.h"
// Entities.
#include "Peripherals/Component2D.h"
#include "OpenGL/Entities/Polygon.h"
#include "OpenGL/Entities/Circle.h"
#include <GLFW/glfw3.h>
#include "OpenGL/Entities/EntityManager.h"
#include "OpenGL/Entities/Circle.h"
#include <iostream>
#include "OpenGL/Buffers/VertexArrayObjectGL.h"
#include "Peripherals/Circuit.h"
// Serialisation.
#include "Utilities/Serialisation/Serialiser.h"
#include "OpenGL/Entities/Text.h"
#include "OpenGL/RendererGL.h"

//=============================================================================================================================================//
//  Press event.																															   //
//=============================================================================================================================================//

void Design2DEngine::mousePressLeft(float pixelCoords[2])
{
	// Call base engine event.
	Base2DEngine::mousePressLeft(pixelCoords);

	glm::vec3 WorldCoords = m_scene->pixelCoordsToWorldCoords(pixelCoords);
	glm::vec2 screenCoords = { WorldCoords.x, WorldCoords.y };

	if (designerState == COMPONENT_PLACE) 
	{
		m_activeComponent->place(screenCoords);
		m_circuit->m_components.insert(m_circuit->m_components.end(), m_activeComponent);
		designerState = ENTITY_SELECT;
	}

	else if (designerState == ENTITY_SELECT) 
	{
		glm::vec2 coords = { pixelCoords[0], pixelCoords[1] };
		m_currentEntityID = getEntityID(coords);

		setActiveComponent(m_currentEntityID);
		setActiveCable(m_currentEntityID);
		Port* clickedPort = getPort(m_currentEntityID);
		if (clickedPort != nullptr) 
		{
			m_activeCable = nullptr;
			designerState = CABLE_PLACE;
			Renderer::bindScene(m_scene.get());
			m_activeCable = std::make_shared<Cable>(clickedPort, m_circuit.get());
		}
	}
	else if (designerState == CABLE_PLACE) 
	{
		glm::vec2 coords = { pixelCoords[0], pixelCoords[1] };
		m_currentEntityID = getEntityID(coords);
		Port* clickedPort = getPort(m_currentEntityID);
		if (clickedPort != nullptr) 
		{
			//Only add the cable if the end port is different to the start port.
			if (clickedPort != m_activeCable->m_startPort) 
			{
				m_activeCable->attach(clickedPort);
				m_circuit->m_cables.push_back(m_activeCable);
			}
			m_activeCable = nullptr;
			designerState = ENTITY_SELECT;
		}
		else m_activeCable->addSegment(screenCoords);
	}
}

void Design2DEngine::mousePressRight(float pixelCoords[2])
{
	// Call base engine event.
	Base2DEngine::mousePressRight(pixelCoords);
	// Update current entity ID.
	glm::vec2 coords = { pixelCoords[0], pixelCoords[1] };
	m_currentEntityID = getEntityID(coords);
	setActiveComponent(m_currentEntityID);
}

void Design2DEngine::mousePressMiddle(float pixelCoords[2])
{
	// Call base engine event.
	Base2DEngine::mousePressMiddle(pixelCoords);
}

//=============================================================================================================================================//
//  Move event.																																   //
//=============================================================================================================================================//

void Design2DEngine::mouseMoveEvent(float pixelCoords[2], int buttonStateLeft, int buttonStateRight, int buttonStateMiddle)
{
	glm::vec2 coords = { pixelCoords[0], pixelCoords[1] };

	glm::vec3 WorldCoords = m_scene->pixelCoordsToWorldCoords(pixelCoords);
	glm::vec2 screenCoords = { WorldCoords.x, WorldCoords.y };
	if (designerState == COMPONENT_PLACE) 
	{
		// Move the component.
		m_activeComponent->moveTo(screenCoords);
	}
	else 
	{
		// Port hover indicator.
		
		m_hoveredID = getEntityID(coords);
		auto lastHoveredPort = m_hoveredPort;
		m_hoveredPort = getPort(m_hoveredID);
		if (m_hoveredPort != lastHoveredPort) 
		{
			if      (m_hoveredPort)   m_hoveredPort->showAttachIndicator();
			else if (lastHoveredPort) lastHoveredPort->hideAttachIndicator();
		}

		if (designerState == CABLE_PLACE)
		{
			m_currentEntityID = getEntityID(coords);
			if (m_activeCable.get()) m_activeCable->extendSegment(screenCoords);
		}
		else if (designerState == ENTITY_SELECT) 
		{
			if (buttonStateLeft) 
			{
				//User is dragging a component.
				glm::vec2 translation = screenCoords - m_lastDragPos;
				if (m_activeComponent.get()) m_activeComponent->move(translation);
				if (m_activeCable.get())     m_activeCable->moveActivePrimitiveTo(screenCoords);
			}

		}
	}
	m_lastDragPos = screenCoords;

	#ifdef _DEBUG
		m_currentEntityID = getEntityID(coords);
		std::cout << m_currentEntityID << std::endl;
	#endif

	// Call parent event.
	Base2DEngine::mouseMoveEvent(pixelCoords, buttonStateLeft, buttonStateRight, buttonStateMiddle);
}

//=============================================================================================================================================//
//  Scroll event.																															   //
//=============================================================================================================================================//

void Design2DEngine::mouseScrollEvent(float pixelCoords[2], float yOffset)
{
	// Call the base engine event.
	Base2DEngine::mouseScrollEvent(pixelCoords, yOffset);
}

//=============================================================================================================================================//
//  Key press.																																   //
//=============================================================================================================================================//

void Design2DEngine::keyEvent(int key, int action)
{
	if (action == GLFW_PRESS) 
	{
		float pixelCoords[] = { m_guiState->renderWindowMouseCoordinate.x, m_guiState->renderWindowMouseCoordinate.y };
		glm::vec3 WorldCoords = m_scene->pixelCoordsToWorldCoords(pixelCoords);
		glm::vec2 screenCoords = { WorldCoords.x, WorldCoords.y };
		switch (key) 
		{
		// --------------------------------------------------------------------------------------------------------------- //
		//case GLFW_KEY_P:
		//	//Enter component placement mode.
			//ComponentPlaceMode(screenCoords);
		//	m_activeCable = nullptr;
		//	break;
		// --------------------------------------------------------------------------------------------------------------- //
		case GLFW_KEY_ESCAPE:
			designerState = ENTITY_SELECT;
			//Remove the dummy component
			m_activeComponent = nullptr;
			m_activeCable = nullptr;
			break;
		// --------------------------------------------------------------------------------------------------------------- //
		case GLFW_KEY_DELETE:
			if ((designerState == ENTITY_SELECT)) 
			{
				deleteActiveComponent();
				deleteActiveCable();
			}
			break;
		}
	}
}

//Helper functions
void Design2DEngine::setActiveComponent(unsigned eID) {

	if (m_activeComponent) 
	{
		m_activeComponent->unhighlight();
		m_activeComponent = NULL;
	}
	if ((eID == 0) || (eID == -1)) 
	{
		m_guiState->clickedZone.background = true;
	}
	else 
	{
		m_guiState->clickedZone.background = false;
		Entity* currentEntity = EntityManager::getEntity(eID);
		currentEntity->setContext(m_guiState);
		while (currentEntity->m_type != EntityType::COMPONENT) 
		{
			currentEntity = currentEntity->m_parent;
			if (currentEntity->m_parent == nullptr) 
			{
				// User did not click on a component. Return.
				return;
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

		// Pass the active component to the GUI state for editing.
		m_guiState->active_component = m_activeComponent.get();
	}
}
void Design2DEngine::setActiveCable(unsigned eID) {

	if (m_activeCable) 
	{
		m_activeCable->unhighlight();
		m_activeCable = NULL;
	}
	if ((eID == 0) || (eID == -1)) {
		m_guiState->clickedZone.background = true;
	}
	else {
		m_guiState->clickedZone.background = false;
		Entity* currentEntity = EntityManager::getEntity(eID);
		currentEntity->setContext(m_guiState);
		while (currentEntity->m_parent != nullptr) {
			if (currentEntity->m_parent->m_type == EntityType::CABLE) {
				//User clicked on a component. Set this cable as active, and send the primative to the component.
				Cable* cur = dynamic_cast<Cable*>(currentEntity->m_parent);
				//m_activeComponent = dynamic_cast<std::shared_ptr>(cur);
				m_activeCable = *std::find_if(begin(m_circuit->m_cables), end(m_circuit->m_cables), [&](std::shared_ptr<Cable> current)
					{
						return current.get() == cur;
					});
				m_activeCable->setActivePrimitive(currentEntity);
				m_activeCable->highlight();
			}
			currentEntity = currentEntity->m_parent;
		}
	}
}

Port* Design2DEngine::getPort(unsigned eID) 
{
	if ((eID == 0) || (eID == -1)) 
	{
		return nullptr;
		m_guiState->clickedZone.background = true;
	}
	else 
	{
		m_guiState->clickedZone.background = false;
		Entity* currentEntity = EntityManager::getEntity(eID);
		while (currentEntity->m_type != EntityType::PORT) 
		{
			currentEntity = currentEntity->m_parent;

			// Return if current entity is NULL.
			if (!currentEntity) return nullptr;
			// Return if entity has no parent.
			if (!currentEntity->m_parent) return nullptr;
		}

		// This cast remains valid provided all entities on screen are decendents of components. If not, this needs to change.
		Port* cur = dynamic_cast<Port*>(currentEntity);
		return cur;
	}
}
//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//