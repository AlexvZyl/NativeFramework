#include "ComponentDesigner.h"
#include "Application/Events/EventLog.h"
#include "Application/Events/Events.h"
#include "Application/ApplicationTemplates.h"
#include "GUI/PopUpMenu/PopUpMenu.h"
#include "Lumen/Lumen.h"
#include "Application/Application.h"
#include "External/GLFW/Includes/GLFW/glfw3.h"
#include "Graphics/OpenGL/Primitives/Polygon.h"
#include "Graphics/OpenGL/Primitives/LineSegment.h"
#include "Graphics/OpenGL/Primitives/Circle.h"
#include "Utilities/Logger/Logger.h"
#include "Graphics/OpenGL/Primitives/PolyLine.h"
#include "GUI/TextEntryGUI/TextEntryGUI.h"
#include "OpenGL/Primitives/Grid.h"
#include "Peripherals/Component2D.h"
#include "OpenGL/SceneGL.h"
#include "Utilities/Serialisation/Serialiser.h"
#include "Utilities/Windows/WindowsUtilities.h"
#include "Peripherals/Port.h"

void ComponentDesigner::onMouseButtonEvent(const MouseButtonEvent& event)
{
	if (event.isType(EventType_MousePress | EventType_MouseButtonLeft))
	{
		glm::vec2 pixelCoords = event.mousePosition;
		glm::vec3 WorldCoords = pixelToWorldCoords(pixelCoords);
		glm::vec2 screenCoords = { WorldCoords.x, WorldCoords.y };


		if (designerState == CompDesignState::SELECT)
		{
			m_currentEntityID = getEntityID(pixelCoords);
			if (m_activePoly || m_activeCircle || m_activeLine) 
			{
				//If we already have an active primitive, we need to check for vertex selection.
				//TODO: Ideally, we should highlight any hovered vertices as well to indicate possible selection.
				setActiveVertex(screenCoords);

			}
			if (m_activeVertexIdx == -1) 
			{
				//If we did not select a vertex, then we can check for a new primitive selection
				setActivePrimitives(m_currentEntityID);
			}
		}
		else if (designerState == CompDesignState::DRAW_POLY)
		{

			if (!m_activePoly)
			{
				if (drawFilled) 
				{
					m_activePoly = Renderer::addPolygon2D({ {getNearestGridVertex(screenCoords), 0.f},{getNearestGridVertex(screenCoords), 0.f} }, penColour, m_activeComponent.get());
				}
				else {
					m_activePoly = Renderer::addPolygon2DClear({ getNearestGridVertex(screenCoords),getNearestGridVertex(screenCoords) }, penThickness, m_activeComponent.get(), penColour);
				}
			}
			else
			{
				m_activePoly->pushVertex({ getNearestGridVertex(screenCoords), 0.f });
			}
		}
		else if (designerState == CompDesignState::DRAW_LINE)
		{
			if (!m_activeLine) 
			{
				//start new line
				bool rounded = false;
				m_activeLine = Renderer::addPolyLine({ getNearestGridVertex(screenCoords), getNearestGridVertex(screenCoords) }, penThickness, penColour, rounded, m_activeComponent.get());
			}
			else {
				//end the line
				m_activeComponent->addLine(m_activeLine);
				m_activeLine = nullptr;
			}
		}
		else if (designerState == CompDesignState::DRAW_CIRCLE)
		{
			if (!m_activeCircle) 
			{
				//start new circle
				if (drawFilled) 
				{
					m_activeCircle = Renderer::addCircle2D(getNearestGridVertex(screenCoords), 0.f, penColour, -1.0f, 0.f, m_activeComponent.get());
				}
				else 
				{
					m_activeCircle = Renderer::addCircle2D(getNearestGridVertex(screenCoords), 0.f, penColour, penThickness, 0.f, m_activeComponent.get());
				}
			}
			else 
			{
				//set the circle radius
				m_activeComponent->addCircle(m_activeCircle);
				m_activeCircle = nullptr;
			}
		}
		else if (designerState == CompDesignState::PLACE_PORT) {
			m_activeComponent->addPort(m_activePort);
			m_activePort = std::make_shared<Port>(getNearestGridVertex(screenCoords), PortType::PORT_INOUT, m_activeComponent.get());
		}
		else if (designerState == CompDesignState::ADD_TEXT)
		{
			// Create a popup GUI for the text entry.
			if (!m_activeText) {
				m_activeText = Renderer::addText2D(" ", screenCoords, textColour, textSize, "C", "B", m_activeComponent.get());
				m_activeComponent->m_text.push_back(m_activeText);
			
				TextEntryGUI* menu = Lumen::getApp().pushWindow<TextEntryGUI>(LumenDockPanel::Floating, "Text Entry", m_activeText);
				switchState(CompDesignState::SELECT);
			}
		}
	}

	if (event.isType(EventType_MousePress | EventType_MouseButtonRight))
	{
			// Create a popup menu on a right click on a graphics scene.
			Lumen::getApp().pushWindow<PopUpMenu>(LumenDockPanel::Floating, "Popup Menu");
	}

	if (event.isType(EventType_MouseDoublePress)) 
	{
		if (designerState == CompDesignState::SELECT) 
		{
			if (m_activeText) Lumen::getApp().pushWindow<TextEntryGUI>(LumenDockPanel::Floating, "Text Entry", m_activeText);
		}
	}
}

void ComponentDesigner::onMouseMoveEvent(const MouseMoveEvent& event)
{
	glm::vec2 screenCoords = pixelToWorldCoords(event.mousePosition);

	if (designerState == CompDesignState::DRAW_POLY)
	{
		// Move the back vertex.
		if (m_activePoly) 
		{
			//hacky check to see if polygon is filled
			PolyLine* polyline = dynamic_cast<PolyLine*>(m_activePoly);
			if (polyline) {
				//Polygon is clear, so we index with m_vertices (nodes)
				polyline->translateVertexAtIndexTo(polyline->m_vertices.size()-1, getNearestGridVertex(screenCoords));
			}
			else {
				//Polygon is filled, so we index like this (using the vertex count)
				m_activePoly->translateVertexAtIndexTo(m_activePoly->m_vertexCount - 1, getNearestGridVertex(screenCoords));
			}
		}
	}
	else if (designerState == CompDesignState::DRAW_LINE)
	{
		if (m_activeLine)
		{
			// Update the line end position.
			m_activeLine->translateVertexAtIndexTo(m_activeLine->m_vertices.size() - 1, getNearestGridVertex(screenCoords));
		}
	}
	else if (designerState == CompDesignState::DRAW_CIRCLE)
	{
		if (m_activeCircle) 
		{
			// Update circle.
			m_activeCircle->setRadius(glm::length(glm::vec2(m_activeCircle->m_trackedCenter) - getNearestGridVertex(screenCoords)));
		}
	}
	else if (designerState == CompDesignState::PLACE_PORT)
	{
		if (m_activePort.get()) {
			//update the title location based on the positioning for user convenience
			if (m_activePort->centre.x >= 0 && getNearestGridVertex(screenCoords).x < 0) {
				m_activePort->titleOffset = -m_activePort->titleOffset;
				m_activePort->title->updateAlignment("R");
			}
			if (m_activePort->centre.x < 0 && getNearestGridVertex(screenCoords).x >= 0) {
				m_activePort->titleOffset = -m_activePort->titleOffset;
				m_activePort->title->updateAlignment("L");
			}
			//update port location
			m_activePort->moveTo(getNearestGridVertex(screenCoords));
		}
	}

	else if (designerState == CompDesignState::SELECT)
	{

		if (m_activePoly || m_activeCircle || m_activeLine)
		{
			//If we already have an active primitive, we need to check for vertex selection.
			//TODO: Ideally, we should highlight any hovered vertices as well to indicate possible selection.
			setHoveredVertex(screenCoords);

		}

		if (event.isNotType(EventType_MouseButtonLeft)) 
		{
			m_lastDragPos = screenCoords;
		}
	}

	// Store state.
	m_currentEntityID = getEntityID(event.mousePosition);
}

void ComponentDesigner::onMouseScrollEvent(const MouseScrollEvent& event)
{
	Base2DEngine::onMouseScrollEvent(event);
	// Everything is in mm.
	float currentCameraScale = (1.f / getScene().getCamera().getTotalScale().x) * 1000.f;
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

void ComponentDesigner::onKeyEvent(const KeyEvent& event)
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
			//Add new polygon
			switchState(CompDesignState::DRAW_POLY);
			break;

		case GLFW_KEY_L:
			//Add new line
			switchState(CompDesignState::DRAW_LINE);
			break;

		case GLFW_KEY_C:
			//Add new circle
			switchState(CompDesignState::DRAW_CIRCLE);
			break;

		case GLFW_KEY_O:
			//Add new port
			switchState(CompDesignState::PLACE_PORT);
			break;

		case GLFW_KEY_ESCAPE:
			if ((designerState == CompDesignState::DRAW_POLY && m_activePoly)|| (designerState == CompDesignState::DRAW_CIRCLE && m_activeCircle)|| (designerState == CompDesignState::DRAW_LINE && m_activeLine)) {
				switchState(designerState);
			}
			else { switchState(CompDesignState::SELECT); }
			break;

		case GLFW_KEY_S:
			/*if (!m_activePoly) return;
			glm::vec2 center = m_activePoly->m_trackedCenter;
			glm::vec2 centerGlobalPixelCoords = localToGlobalCoords(worldToPixelCoords(center));
			EventLog::logMouseScroll(centerGlobalPixelCoords, 1.f, 0.f, 0);
			EventLog::logMouseDrag(centerGlobalPixelCoords, centerGlobalPixelCoords + glm::vec2{ 100.f, 0.f }, { 100.f, 0.f }, EventType_MouseButtonLeft);
			glm::vec2 expectedPos = center + glm::vec2(pixelToWorldDistance({100.f, 0.f}));
			Renderer::addCircle2D(glm::vec3{ glm::vec2(expectedPos), 0.6f }, 0.01f, { 1.f, 0.f, 1.f, 1.f });*/
			// Red = tracked center
			// Pink = expected position
			break;


		case GLFW_KEY_T:
			//Add new text
			switchState(CompDesignState::ADD_TEXT);
			break;

		case GLFW_KEY_DELETE:
			if (designerState == CompDesignState::SELECT) deleteActivePrimitive();
			break;

		case GLFW_KEY_BACKSPACE:
			if (designerState == CompDesignState::SELECT) deleteActivePrimitive();
			break;

		case GLFW_KEY_ENTER:
			if (designerState != CompDesignState::SELECT) {
				pushActivePrimitives();
				switchState(CompDesignState::SELECT);
			}
			break;
		}
	}
}

void ComponentDesigner::onMouseDragEvent(const MouseDragEvent& event) 
{
	Base2DEngine::onMouseDragEvent(event);

	if (event.isType(EventType_MouseButtonLeft) && event.isNotType(EventType_MouseButtonLeft | EventType_SpaceBar))
	{
		glm::vec2 pixelCoords = event.mousePosition;
		glm::vec3 WorldCoords = pixelToWorldCoords(pixelCoords);
		glm::vec2 screenCoords = { WorldCoords.x, WorldCoords.y };
		if (designerState == CompDesignState::SELECT) 
		{
			// User is dragging a component.
			glm::vec2 translation = pixelToWorldDistance(event.currentFrameDelta);
			if (m_activeVertexIdx != -1) 
			{
				// First check if we should move a vertex
				// We need to move a vertex
				if (m_activePoly) 
				{
					m_activePoly->translateVertexAtIndex(m_activeVertexIdx, translation);
				}
				else if (m_activeLine) 
				{
					m_activeLine->translateVertexAtIndex(m_activeVertexIdx, translation);
				}
			}
			else 
			{
				// If we are not moving a vertex, then check to move primitives
				if (m_activePoly) 
				{
					m_activePoly->translate(translation);
					m_lastDragPos = screenCoords;
				}
				if (m_activeLine) 
				{
					m_activeLine->translate(translation);
					m_lastDragPos = getNearestGridVertex(screenCoords);
				}
				if (m_activeCircle) 
				{
					m_activeCircle->translate(translation);
					m_lastDragPos = getNearestGridVertex(screenCoords);
				}
				if (m_activeText) 
				{
					// Consideration: Should we keep track of the text position in the parent port/component? If so, this should be updated here.
					m_activeText->translate(translation);
					m_lastDragPos = getNearestGridVertex(screenCoords);
				}
				if (m_activePort.get()) 
				{
					m_activePort->move(translation);
					m_lastDragPos = getNearestGridVertex(screenCoords);
				}
			}
		}
	}
}

void ComponentDesigner::onNotifyEvent(const NotifyEvent& event) 
{
	if (event.isType(EventType_MouseDragStop | EventType_MouseButtonLeft))
	{
		if (m_activePoly)		   m_activePoly->translateTo(getNearestGridVertex(m_activePoly->m_trackedCenter));
		if (m_activeLine)		   m_activeLine->translateTo(getNearestGridVertex(m_activeLine->m_trackedCenter));
		if (m_activeCircle)		   m_activeCircle->translateTo(getNearestGridVertex(m_activeCircle->m_trackedCenter));
		if (m_activeVertexIdx != -1) 
		{
			if (m_activeLine) m_activeLine->translateVertexAtIndexTo(m_activeVertexIdx, getNearestGridVertex(m_activeLine->m_vertices.at(m_activeVertexIdx)));
			else if (m_activePoly) 
			{
				PolyLine* polyline = dynamic_cast<PolyLine*>(m_activePoly);
				if (polyline) 
				{
					// Polygon is clear, so we index with m_vertices (nodes).
					polyline->translateVertexAtIndexTo(m_activeVertexIdx, getNearestGridVertex(polyline->m_vertices.at(m_activeVertexIdx)));
				}
				else 
				{
					m_activePoly->translateVertexAtIndexTo(m_activeVertexIdx, getNearestGridVertex(m_activePoly->getVertex(m_activeVertexIdx).position));
				}
			}
		}
		if (m_activePort)		   m_activePort->moveTo(getNearestGridVertex(m_activePort->centre));
	}
}

void ComponentDesigner::onFileSaveEvent(const FileSaveEvent& event) 
{
	// Save to existing file.
	if (event.saveAs)
	{
		// Iterate through the paths.
		for (auto& path : event.fileData)
		{
			// Check if operation did not fail.
			if (path.string().size())
			{
				saveToYAML(m_activeComponent.get(), path);
				setName(path.filename().stem().string());
				if (path.extension().string() != ".lmcp")
				{
					path.extension() = ".lmcp";
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
			savePath = selectFile("Lumen Save Circuit", "", m_activeComponent->title->m_string, "Save"); 
			if (savePath.extension().string() != ".lmcp")
			{
				savePath.extension() = ".lmcp";
			}
		}
		if (savePath.string().size())
		{
			saveToYAML(m_activeComponent.get(), savePath);
			setName(savePath.filename().stem().string());
			savedDocument();
		}
	}
}