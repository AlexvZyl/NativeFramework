//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <iostream>
#include "Engines/Design2DEngine/Design2DEngine.h"
#include "Engines/Design2DEngine/Peripherals/Component2D.h"
#include "Engines/Design2DEngine/Peripherals/Cable.h"
#include "Engines/Design2DEngine/Peripherals/Circuit.h"
#include "Application/Events/Events.h"
#include "OpenGL/SceneGL.h"
#include "External/GLFW/Includes/GLFW/glfw3.h"
#include "GUI/PopUpMenu/PopUpMenu.h"
#include "Lumen.h"
#include "Application/Application.h"

//==============================================================================================================================================//
//  Mouse Button.																																//
//==============================================================================================================================================//

void Design2DEngine::onMouseButtonEvent(MouseButtonEvent& event)
{
	Base2DEngine::onMouseButtonEvent(event);

	uint64_t eventID = event.ID;

	// --------------------- //
	//  L E F T   P R E S S  //
	// --------------------- //

	if (eventID == (EventType_MousePress | EventType_MouseButtonLeft))
	{
		glm::vec2 pixelCoords = event.mousePosition;
		float pixelCoordsf[2] = { pixelCoords.x, pixelCoords.y };
		glm::vec3 WorldCoords = m_scene->pixelCoordsToWorldCoords(pixelCoordsf);
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
				// Only add the cable if the end port is different to the start port.
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

	// ----------------------- //
	//  R I G H T   P R E S S  //
	// ----------------------- //

	else if (eventID == (EventType_MousePress | EventType_MouseButtonRight))
	{

		//Switch out of any placement state
		if (designerState == COMPONENT_PLACE || designerState == CABLE_PLACE) {
			m_activeComponent = nullptr;
			m_activeCable = nullptr;
			designerState = ENTITY_SELECT;
		}
		else {

			// Update current entity ID.
			m_currentEntityID = getEntityID(event.mousePosition);
			setActiveComponent(m_currentEntityID);


			// Create a popup menu on a right click on a graphics scene.
			PopUpMenu* menu = Lumen::getApp().pushGuiLayer<PopUpMenu>("Popup Menu", DockPanel::Floating)->getGui();
			glm::vec2 pos = {

				event.mousePosition.x + m_contentRegionPos.x,
				event.mousePosition.y + m_contentRegionPos.y
			};
			menu->setInitialPosition(pos);
			menu->setEngine(this);
		}
	}

	// ------------------------- //
	//  M I D D L E   P R E S S  //
	// ------------------------- //

	else if (eventID == (EventType_MousePress | EventType_MouseButtonMiddle))
	{

	}
}

//==============================================================================================================================================//
//  Mouse Move.																																	//
//==============================================================================================================================================//

void Design2DEngine::onMouseMoveEvent(MouseMoveEvent& event)
{
	Base2DEngine::onMouseMoveEvent(event);

	uint64_t eventID = event.ID;

	glm::vec2 coords = event.mousePosition;
	float coordsf[2] = { coords.x, coords.y };
	glm::vec3 WorldCoords = m_scene->pixelCoordsToWorldCoords(coordsf);
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
		if (m_hoveredPort != lastHoveredPort && m_hoveredPort)
		{
			if (m_hoveredPort)   m_hoveredPort->showAttachIndicator();
			else if (lastHoveredPort) lastHoveredPort->hideAttachIndicator();
		}

		if (designerState == CABLE_PLACE)
		{
			m_currentEntityID = getEntityID(coords);
			if (m_activeCable.get()) m_activeCable->extendSegment(screenCoords);
		}
		else if (designerState == ENTITY_SELECT)
		{
			if (eventID == EventType_MouseButtonLeft)
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
}

//==============================================================================================================================================//
//  Mouse Scrol.																																//
//==============================================================================================================================================//

void Design2DEngine::onMouseScrollEvent(MouseScrollEvent& event)
{
	Base2DEngine::onMouseScrollEvent(event);
}

//==============================================================================================================================================//
//  Key Events.																																	//
//==============================================================================================================================================//

void Design2DEngine::onKeyEvent(KeyEvent& event)
{
	Base2DEngine::onKeyEvent(event);

	// Get the event ID.
	uint64_t eventID = event.ID;

	// Events based on key type.
	if (eventID == EventType_KeyPress) 
	{
		// Event mouse coordinates.
		glm::vec2 pixelCoords = event.mousePosition;
		float pixelCoordsf[] = { pixelCoords.x, pixelCoords.y };
		glm::vec3 WorldCoords = m_scene->pixelCoordsToWorldCoords(pixelCoordsf);
		glm::vec2 screenCoords = { WorldCoords.x, WorldCoords.y };

		switch (event.key)
		{
			// --------------------------------------------------------------------------------------------------------------- //
			
			case GLFW_KEY_P:
				// Enter component placement mode.
				ComponentPlaceMode(screenCoords);
				m_activeCable = nullptr;
				break;
			
			// --------------------------------------------------------------------------------------------------------------- //

			case GLFW_KEY_ESCAPE:
				designerState = ENTITY_SELECT;
				// Remove the dummy component.
				m_activeComponent = nullptr;
				m_activeCable = nullptr;
				break;

			// --------------------------------------------------------------------------------------------------------------- //

			case GLFW_KEY_DELETE:
				if (designerState == ENTITY_SELECT)
				{
					deleteActiveComponent();
					deleteActiveCable();
				}
				break;
		}
	}
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//