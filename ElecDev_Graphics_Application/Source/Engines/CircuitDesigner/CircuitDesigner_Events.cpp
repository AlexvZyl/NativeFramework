//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <iostream>
#include "Engines/CircuitDesigner/CircuitDesigner.h"
#include "Engines/CircuitDesigner/Peripherals/Component2D.h"
#include "Engines/CircuitDesigner/Peripherals/Cable.h"
#include "Engines/CircuitDesigner/Peripherals/Circuit.h"
#include "Application/Events/Events.h"
#include "Application/ApplicationTemplates.h"
#include "OpenGL/SceneGL.h"
#include "External/GLFW/Includes/GLFW/glfw3.h"
#include "GUI/PopUpMenu/PopUpMenu.h"
#include "Lumen/Lumen.h"
#include "Application/Application.h"
#include "Utilities/Logger/Logger.h"
#include "glm/glm.hpp"
#include "OpenGL/Primitives/Grid.h"
#include "GUI/LumenGizmo/LumenGizmo.h"
#include "Utilities/Serialisation/Serialiser.h"
#include "Utilities/Windows/WindowsUtilities.h"
#include "Engines/CircuitDesigner/Peripherals/Port.h"

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
				if (worldToPixelDistance({ distance, 0.f, 0.f }).x < clickTol)
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
				//attach if port is valid
				if (m_activeCable->attach(clickedPort)) {
					m_circuit->m_cables.push_back(m_activeCable);
					m_activeCable->disableOutline();
					m_activeCable = nullptr;
					m_activeComponent->disableOutline();
					m_activeComponent = nullptr;

					designerState = ENTITY_SELECT;
				}
				else {
					Lumen::getApp().pushNotification(NotificationType::Info, 1000, "Check input/output relationships.", "Invalid Port Combination");
				}
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
			m_activeVertexIdx = -1;
			designerState = ENTITY_SELECT;
		}
		else 
		{
			// Update current entity ID.
			m_currentEntityID = getEntityID(event.mousePosition);

				m_activeVertexIdx = -1;
				setActiveComponent(m_currentEntityID);
				setActiveCable(m_currentEntityID);

			// Create a popup menu on a right click on a graphics scene.
			PopUpMenu* menu = Lumen::getApp().pushWindow<PopUpMenu>(LumenDockPanel::Floating, "Popup Menu");
		}
	}

	// Set the entity the gizmo has to edit.
	if (m_activeComponent) getGizmo().setEntity(m_activeComponent.get());
	else				   getGizmo().clearEntities();
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
		else if (designerState == ENTITY_SELECT)
		{
			if (m_activeCable.get()) {
				auto [idx, distance] = m_activeCable->getNearestVertexIdx(screenCoords);
				m_hoveredVertexIdx = -1;
				if (worldToPixelDistance({ distance, 0.f, 0.f }).x < clickTol)
				{
					m_hoveredVertexIdx = idx;
				}
			}
		}
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
	// Everything is in mm.
	float currentCameraScale = ( 1.f / getScene().getCamera().getTotalScale().x ) * 1000.f;
	float currentGridSize = getScene().getGrid().getCoarseIncrementSize() * 1000.f;
	float gridIncrementFactor = 5.f;
	float minimumGridSize = 1.f;
	if (currentCameraScale < currentGridSize)
	{
		if (currentGridSize / gridIncrementFactor < minimumGridSize)
		{
			getScene().getGrid().setMajorGrid(GridUnit::MILLIMETER, minimumGridSize);
		}
		else
		{
			getScene().getGrid().setMajorGrid(GridUnit::MILLIMETER, currentGridSize / gridIncrementFactor);
		}
	}
	else if (currentCameraScale > currentGridSize * gridIncrementFactor)
	{
		getScene().getGrid().setMajorGrid(GridUnit::MILLIMETER, currentGridSize * gridIncrementFactor);
	}
}

//==============================================================================================================================================//
//  Mouse Drag.																																    //
//==============================================================================================================================================//

void CircuitDesigner::onMouseDragEvent(const MouseDragEvent& event)
{
	Base2DEngine::onMouseDragEvent(event);

	if (event.isType(EventType_MouseButtonLeft) && event.isNotType(EventType_SpaceBar | EventType_MouseButtonLeft))
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
//  Notify event.																																//
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

void CircuitDesigner::onFileSaveEvent(const FileSaveEvent& event) 
{
	if (event.saveAs)
	{
		// Iterate through the paths.
		for (auto& path : event.fileData)
		{
			// Check if operation did not fail.
			if (path.string().size())
			{
				saveToYAML(m_circuit.get(), path);
				setName(path.filename().stem().string());
				if (path.filename().extension().string() != ".lmct")
				{
					path.extension() = ".lmct";
				}
				savePath = path;
				savedDocument();
			}
		}
	}	
	// Save to new file.
	else {
		if (!savePath.string().size()) {
			// Get save path.
			savePath = selectFile("Lumen Save Circuit", "", m_circuit->m_label, "Save"); 
			if (savePath.filename().extension().string() != ".lmct")
			{
				savePath.extension() = ".lmct";
			}
		}
		if (savePath.string().size())
		{
			saveToYAML(m_circuit.get(), savePath);
			setName(savePath.filename().stem().string());
			savedDocument();
		}
	}
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//