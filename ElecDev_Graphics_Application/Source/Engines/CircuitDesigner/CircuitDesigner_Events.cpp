//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <iostream>
#include "Engines/CircuitDesigner/CircuitDesigner.h"
#include "Engines/CircuitDesigner/Peripherals/Component2D.h"
#include "Engines/CircuitDesigner/Peripherals/Cable.h"
#include "Engines/CircuitDesigner/Peripherals/Circuit.h"
#include "Application/Events/Events.h"
#include "OpenGL/SceneGL.h"
#include "External/GLFW/Includes/GLFW/glfw3.h"
#include "GUI/PopUpMenu/PopUpMenu.h"
#include "Lumen.h"
#include "Application/Application.h"
#include "Utilities/Logger/Logger.h"
#include "glm/glm.hpp"

//==============================================================================================================================================//
//  Mouse Button.																																//
//==============================================================================================================================================//

void CircuitDesigner::onMouseButtonEvent(const MouseButtonEvent& event)
{
	// --------------------- //
	//  L E F T   P R E S S  //
	// --------------------- //

	if (event.isType(EventType_MousePress | EventType_MouseButtonLeft))
	{
		glm::vec2 pixelCoords = event.mousePosition;
		glm::vec3 WorldCoords = pixelToWorldCoords(pixelCoords);
		glm::vec2 screenCoords = { WorldCoords.x, WorldCoords.y };

		if (designerState == COMPONENT_PLACE)
		{
			m_activeComponent->place(getNearestGridVertex(screenCoords));
			m_circuit->m_components.insert(m_circuit->m_components.end(), m_activeComponent);
			designerState = ENTITY_SELECT;
		}

		else if (designerState == ENTITY_SELECT)
		{
			glm::vec2 coords = { pixelCoords[0], pixelCoords[1] };
			m_currentEntityID = getEntityID(coords);

			if (m_activeCable.get()) {
				auto [idx, distance] = m_activeCable->getNearestVertexIdx(screenCoords);
				m_activeVertexIdx = -1;
				if (distance < clickTol)
				{
					m_activeVertexIdx = idx;
				}
			}
			if (m_activeVertexIdx == -1) {
				setActiveComponent(m_currentEntityID);
				setActiveCable(m_currentEntityID);
				Port* clickedPort = getPort(m_currentEntityID);
				if (clickedPort)
				{
					m_activeCable = nullptr;
					designerState = CABLE_PLACE;
					m_activeCable = std::make_shared<Cable>(clickedPort, m_circuit.get());
					m_activeCable->enableOutline();
					m_activeComponent->disableOutline();
				}
			}
		}
		else if (designerState == CABLE_PLACE)
		{
			glm::vec2 coords = { pixelCoords[0], pixelCoords[1] };
			m_currentEntityID = getEntityID(coords);
			Port* clickedPort = getPort(m_currentEntityID);

			if (clickedPort)
			{
				// Only add the cable if the end port is different to the start port.
				if (clickedPort != m_activeCable->m_startPort)
				{
					m_activeCable->attach(clickedPort);
					m_circuit->m_cables.push_back(m_activeCable);
				}
				m_activeCable->disableOutline();
				m_activeCable = nullptr;
				m_activeComponent->disableOutline();
				m_activeComponent = nullptr;

				designerState = ENTITY_SELECT;
			}
			else
			{
				m_activeCable->addSegment(getNearestGridVertex(screenCoords));
				m_activeCable->enableOutline();
			}
		}

		LUMEN_LOG_DEBUG(std::to_string(getEntityID(event.mousePosition)), "Design Engine EID");
	}

	// ----------------------- //
	//  R I G H T   P R E S S  //
	// ----------------------- //

	else if (event.isType(EventType_MousePress | EventType_MouseButtonRight))
	{

		// Switch out of any placement state.
		if (designerState == COMPONENT_PLACE || designerState == CABLE_PLACE) 
		{
			m_activeComponent = nullptr;
			m_activeCable = nullptr;
			designerState = ENTITY_SELECT;
		}
		else 
		{
			// Update current entity ID.
			m_currentEntityID = getEntityID(event.mousePosition);
			setActiveComponent(m_currentEntityID);
			setActiveCable(m_currentEntityID);

			// Create a popup menu on a right click on a graphics scene.
			PopUpMenu* menu = Lumen::getApp().pushWindow<PopUpMenu>(LumenDockPanel::Floating, "Popup Menu");
		}
	}
}

//==============================================================================================================================================//
//  Mouse Move.																																	//
//==============================================================================================================================================//

void CircuitDesigner::onMouseMoveEvent(const MouseMoveEvent& event)
{
	glm::vec2 coords = event.mousePosition;
	glm::vec3 WorldCoords = pixelToWorldCoords(coords);
	glm::vec2 screenCoords = { WorldCoords.x, WorldCoords.y };
	if (designerState == COMPONENT_PLACE)
	{
		// Move the component.
		m_activeComponent->moveTo(getNearestGridVertex(screenCoords));
	}
	else
	{
		// Port hover indicator.
		m_hoveredID = getEntityID(coords);
		auto lastHoveredPort = m_hoveredPort;
		m_hoveredPort = getPort(m_hoveredID);
		if (m_hoveredPort != lastHoveredPort)
		{
			if (m_hoveredPort)
			{
				m_hoveredPort->showAttachIndicator();
			}
			else if (lastHoveredPort) 
			{
				lastHoveredPort->hideAttachIndicator();
			}
		}

		if (designerState == CABLE_PLACE)
		{
			m_currentEntityID = getEntityID(coords);
			if (m_activeCable.get()) m_activeCable->extendSegment(getNearestGridVertex(screenCoords));
			m_activeCable->enableOutline();
		}
		/*/else if (designerState == ENTITY_SELECT)
		{
			if (event.isType(EventType_MouseButtonLeft))
			{
				//User is dragging a component.
				glm::vec2 translation = screenCoords - m_lastDragPos;
				if (m_activeComponent.get()) m_activeComponent->moveTo(getNearestGridVertex(screenCoords));
				if (m_activeCable.get())     m_activeCable->moveActivePrimitiveTo(getNearestGridVertex(screenCoords));
			}
		}
		*/
	}

	// Store state.
	m_lastDragPos = screenCoords;
	m_currentEntityID = getEntityID(coords);
}

//==============================================================================================================================================//
//  Mouse Scroll.																																//
//==============================================================================================================================================//

void CircuitDesigner::onMouseScrollEvent(const MouseScrollEvent& event)
{
	Base2DEngine::onMouseScrollEvent(event);
}

//==============================================================================================================================================//
//  Mouse Drag.																																//
//==============================================================================================================================================//

void CircuitDesigner::onMouseDragEvent(const MouseDragEvent& event)
{
	Base2DEngine::onMouseDragEvent(event);

	if (event.isType(EventType_MouseButtonLeft))
	{
		glm::vec2 translation = pixelToWorldDistance(event.currentFrameDelta);
		if (designerState == ENTITY_SELECT)
		{
			if (m_activeComponent.get())
			{
				m_activeComponent->move(translation);
			}
			if (m_activeVertexIdx != -1) 
			{
				m_activeCable->translateVertexAtIndex(m_activeVertexIdx, translation);
			}
		}
	}
}

//==============================================================================================================================================//
//  Notify event.																																	//
//==============================================================================================================================================//

void CircuitDesigner::onNotifyEvent(const NotifyEvent& event)
{
	if (event.isType(EventType_MouseDragStop | EventType_MouseButtonLeft))
	{
		if (m_activeComponent.get()) 
		{
			glm::vec2 vert = getNearestGridVertex(m_activeComponent->centre);
			LUMEN_LOG_DEBUG(std::to_string(vert.x), "Component Designer Notify");
			m_activeComponent->moveTo(getNearestGridVertex(m_activeComponent->centre));
		}
		if (m_activeVertexIdx != -1) 
		{
			glm::vec2 worldCoords = pixelToWorldCoords(getMouseLocalPosition());
			m_activeCable->translateVertexAtIndexTo(m_activeVertexIdx, getNearestGridVertex(worldCoords));
		}
	}
}

//==============================================================================================================================================//
//  Key Events.																																	//
//==============================================================================================================================================//

void CircuitDesigner::onKeyEvent(const KeyEvent& event)
{
	// Events based on key type.
	if (event.isType(EventType_KeyPress))
	{
		// Event mouse coordinates.
		glm::vec2 pixelCoords = event.mousePosition;
		glm::vec3 WorldCoords = pixelToWorldCoords(pixelCoords);
		glm::vec2 screenCoords = { WorldCoords.x, WorldCoords.y };

		switch (event.key)
		{
			case GLFW_KEY_P:
				// Enter component placement mode.
				ComponentPlaceMode(screenCoords);
				m_activeCable = nullptr;
				break;

			case GLFW_KEY_ESCAPE:
				// Remove the dummy component.
				designerState = ENTITY_SELECT;
				m_activeComponent = nullptr;
				m_activeCable = nullptr;
				break;

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
//  Files.																																		//
//==============================================================================================================================================//

void CircuitDesigner::onFileDropEvent(const FileDropEvent& event) 
{
	for (auto& path : event.fileData)
	{
		if (path.extension().string() == ".lmcp")
		{
			// Reset active entities.
			if (m_activeComponent) m_activeComponent->disableOutline();
			if (m_activeCable)	   m_activeCable->disableOutline();
			// Import and load component.
			importComponent(path, true);
		}
	}
}

void CircuitDesigner::onYamlNodeDropEvent(const YamlNodeDropEvent& event)
{
	YAML::Node node = event.getNode();
	if (node["Component"].IsDefined()) node = node["Component"];
	// Check type.
	if (node["Filename"].IsDefined())
	{
		std::filesystem::path file(node["Filename"].as<std::string>());
		bool checkForOverwrite = true;
		if (CircuitDesigner::s_engineUsedByCircuitEditor == this) checkForOverwrite = false;
		if (file.extension() == ".lmcp") importComponent(node, true, checkForOverwrite);
	}
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//