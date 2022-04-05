#include "ComponentDesigner.h"
#include "Application/Events/Events.h"
#include "GUI/PopUpMenu/PopUpMenu.h"
#include "Lumen.h"
#include "Application/Application.h"
#include "External/GLFW/Includes/GLFW/glfw3.h"
#include "Graphics/OpenGL/Primitives/Polygon.h"
#include "Graphics/OpenGL/Primitives/LineSegment.h"
#include "Graphics/OpenGL/Primitives/Circle.h"
#include "Utilities/Logger/Logger.h"

void ComponentDesigner::onMouseButtonEvent(MouseButtonEvent& event)
{
	Base2DEngine::onMouseButtonEvent(event);

	if (event.isType(EventType_MousePress | EventType_MouseButtonLeft))
	{
		glm::vec2 pixelCoords = event.mousePosition;
		glm::vec3 WorldCoords = m_scene->pixelCoordsToWorldCoords(pixelCoords);
		glm::vec2 screenCoords = { WorldCoords.x, WorldCoords.y };


		if (designerState == CompDesignState::SELECT)
		{
			m_currentEntityID = getEntityID(pixelCoords);
			setActivePrimitives(m_currentEntityID);
		}
		else if (designerState == CompDesignState::DRAW_POLY)
		{

			if (!m_activePoly)
			{
				m_activePoly = Renderer::addPolygon2D({ {getNearestGridVertex(screenCoords), 0.f},{getNearestGridVertex(screenCoords), 0.f} }, m_activeComponent.get());
				m_activeComponent->addPoly(m_activePoly);
				//m_activePoly->pushVertex({ getNearestGridVertex(screenCoords), 0.f });
			}
			else
			{
				m_activePoly->pushVertex({ getNearestGridVertex(screenCoords), 0.f });
			}
			// Example!
			auto [vertexPtr, distance] = m_activePoly->getNearestVertex(screenCoords);
		}
		else if (designerState == CompDesignState::DRAW_LINE)
		{
			if (!m_activeLine) {
				//start new line
				m_activeLine = Renderer::addLineSegment2D(getNearestGridVertex(screenCoords), getNearestGridVertex(screenCoords), 0.001f, { 0.f, 0.f, 0.f, 1.f }, m_activeComponent.get());
			}
			else {
				//end the line
				m_activeLine->setEnd(getNearestGridVertex(screenCoords));
				m_activeComponent->addLine(m_activeLine);
				//m_activeComponent->addLine(getNearestGridVertex(screenCoords), getNearestGridVertex(screenCoords));
				m_activeLine = nullptr;
			}
		}
		else if (designerState == CompDesignState::DRAW_CIRCLE)
		{
			if (!m_activeCircle) 
			{
				//start new line
				m_activeCircle = Renderer::addCircle2D(getNearestGridVertex(screenCoords), 0.f, m_activeComponent->shapeColour, 1.0f, 0.f, m_activeComponent.get());
			}
			else 
			{
				//end the line
				m_activeComponent->addCircle(m_activeCircle);
				m_activeCircle = nullptr;
			}
		}
		else if (designerState == CompDesignState::PLACE_PORT) {
			m_activeComponent->addPort(m_activePort);
			m_activePort = std::make_shared<Port>(getNearestGridVertex(screenCoords), next_port_type, m_activeComponent.get());
		}
	}

	if (event.isType(EventType_MousePress | EventType_MouseButtonRight))
	{
			// Create a popup menu on a right click on a graphics scene.
			PopUpMenu* menu = Lumen::getApp().pushGuiLayer<PopUpMenu>("Popup Menu", LumenDockPanel::Floating)->getGui();
			glm::vec2 pos = {

				event.mousePosition.x + m_contentRegionPos.x,
				event.mousePosition.y + m_contentRegionPos.y
			};
			menu->setInitialPosition(pos);
			menu->setEngine(this);
	}

	if (event.isType(EventType_MouseDoublePress))
	{
		if (event.isType(EventType_MouseButtonLeft))
		{
			LUMEN_LOG_DEBUG("Left Double Press", "Component Designer Button Event");
		}
		else if (event.isType(EventType_MouseButtonRight))
		{
			LUMEN_LOG_DEBUG("Right Double Press", "Component Designer Button Event");
		}
		else if (event.isType(EventType_MouseButtonMiddle))
		{
			LUMEN_LOG_DEBUG("Middle Double Press", "Component Designer Button Event");
		}
	}
}

void ComponentDesigner::onMouseMoveEvent(MouseMoveEvent& event)
{
	Base2DEngine::onMouseMoveEvent(event);
	uint64_t eventID = event.ID;

	glm::vec2 pixelCoords = event.mousePosition;
	glm::vec3 WorldCoords = m_scene->pixelCoordsToWorldCoords(pixelCoords);
	glm::vec2 screenCoords = { WorldCoords.x, WorldCoords.y };

	if (designerState == CompDesignState::DRAW_POLY)
	{
		// Move the back vertex.
		if (m_activePoly) 
		{
			m_activePoly->translateToVertexAtIndex(m_activePoly->m_vertexCount-1, getNearestGridVertex(screenCoords));
		}
	}
	else if (designerState == CompDesignState::DRAW_LINE)
	{
		if (m_activeLine)
		{
			// Update the line end position.
			m_activeLine->setEnd(getNearestGridVertex(screenCoords));
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
		if (eventID == EventType_MouseButtonLeft)
		{
			//User is dragging a component.
			glm::vec2 translation = screenCoords - m_lastDragPos;
			if (m_activePoly) {
				if (getNearestGridVertex(glm::vec2{ m_activePoly->m_trackedCenter } + translation) != glm::vec2{ m_activePoly->m_trackedCenter }) {
					m_activePoly->translateTo(getNearestGridVertex(glm::vec2{ m_activePoly->m_trackedCenter } + translation));
					m_lastDragPos = getNearestGridVertex(screenCoords);
				}
			}
			if (m_activeLine) {
				if (getNearestGridVertex(glm::vec2{ m_activeLine->m_trackedCenter } + translation) != glm::vec2{ m_activeLine->m_trackedCenter }) {
					m_activeLine->translateTo(getNearestGridVertex(glm::vec2{ m_activeLine->m_trackedCenter } + translation));
					m_lastDragPos = getNearestGridVertex(screenCoords);
				}
			}
			if (m_activeCircle) {
				if (getNearestGridVertex(glm::vec2{ m_activeCircle->m_trackedCenter } + translation) != glm::vec2{ m_activeCircle->m_trackedCenter }) {
					m_activeCircle->translateTo(getNearestGridVertex(glm::vec2{ m_activeCircle->m_trackedCenter } + translation));
					m_lastDragPos = getNearestGridVertex(screenCoords);
				}
			}
			if (m_activePort.get()) {
				if (getNearestGridVertex(glm::vec2{ m_activePort->centre } + translation) != glm::vec2{ m_activePort->centre }) {
					m_activePort->moveTo(getNearestGridVertex(glm::vec2{ m_activePort->centre } + translation));
					m_lastDragPos = getNearestGridVertex(screenCoords);
				}
			}
		}
		else {
			m_lastDragPos = screenCoords;
		}
	}

	// Store state.
	//m_lastDragPos = screenCoords;
	m_currentEntityID = getEntityID(pixelCoords);

	if (event.isType(EventType_MouseDrag))
	{
		LUMEN_LOG_DEBUG("Dragging...","Move Event");
	}
	else 
	{
		LUMEN_LOG_DEBUG("Not dragging.", "Move Event");
	}
}

void ComponentDesigner::onMouseScrollEvent(MouseScrollEvent& event)
{
	Base2DEngine::onMouseScrollEvent(event);
}

void ComponentDesigner::onKeyEvent(KeyEvent& event)
{
	Base2DEngine::onKeyEvent(event);


	// Events based on key type.
	if (event.isType(EventType_KeyPress))
	{
		// Event mouse coordinates.
		glm::vec2 pixelCoords = event.mousePosition;
		glm::vec3 WorldCoords = m_scene->pixelCoordsToWorldCoords(pixelCoords);
		glm::vec2 screenCoords = { WorldCoords.x, WorldCoords.y };

		switch (event.key)
		{
			// --------------------------------------------------------------------------------------------------------------- //

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
			// --------------------------------------------------------------------------------------------------------------- //

		case GLFW_KEY_ESCAPE:
			switchState(CompDesignState::SELECT);
			break;

			// --------------------------------------------------------------------------------------------------------------- //

		case GLFW_KEY_DELETE:
			break;

			// --------------------------------------------------------------------------------------------------------------- //
		}
	}
}

void ComponentDesigner::onMouseDragEvent(MouseDragEvent& event) 
{

}

void ComponentDesigner::onNotifyEvent(NotifyEvent& event) 
{
	if (event.isType(EventType_MouseDragStart))
	{
		LUMEN_LOG_DEBUG("Mouse Drag Start","Component Designer Notify");
	}
	else if (event.isType(EventType_MouseDragStop))
	{
		LUMEN_LOG_DEBUG("Mouse Drag Stop", "Component Designer Notify");
	}
}