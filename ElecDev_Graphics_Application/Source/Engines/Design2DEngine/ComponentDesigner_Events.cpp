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
		glm::vec3 WorldCoords = pixelCoordsToWorldCoords(pixelCoords);
		glm::vec2 screenCoords = { WorldCoords.x, WorldCoords.y };


		if (designerState == CompDesignState::SELECT)
		{
			m_currentEntityID = getEntityID(pixelCoords);
			if (m_activePoly || m_activeCircle || m_activeLine) {
				//If we already have an active primitive, we need to check for vertex selection.
				//TODO: Ideally, we should highlight any hovered vertices as well to indicate possible selection.
				setActiveVertex(screenCoords);

			}
			if (!m_activeVertex) {
				//If we did not select a vertex, then we can check for a new primitive selection
				setActivePrimitives(m_currentEntityID);
			}
		}
		else if (designerState == CompDesignState::DRAW_POLY)
		{

			if (!m_activePoly)
			{
				if (drawFilled) {
					m_activePoly = Renderer::addPolygon2D({ {getNearestGridVertex(screenCoords), 0.f},{getNearestGridVertex(screenCoords), 0.f} }, m_activeComponent.get());
				}
				else {
					m_activePoly = Renderer::addPolygon2DClear({ {getNearestGridVertex(screenCoords), 0.f},{getNearestGridVertex(screenCoords), 0.f} }, m_activeComponent.get());
				}
				//m_activeComponent->addPoly(m_activePoly);
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
				//start new circle
				if (drawFilled) {
					m_activeCircle = Renderer::addCircle2D(getNearestGridVertex(screenCoords), 0.f, m_activeComponent->shapeColour, 1.0f, 0.f, m_activeComponent.get());
				}
				else {
					m_activeCircle = Renderer::addCircle2D(getNearestGridVertex(screenCoords), 0.f, { 0.f, 0.f, 0.f, 1.f }, .02f, 0.f, m_activeComponent.get());
				}
			}
			else 
			{
				//set the circle rarius
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
	glm::vec3 WorldCoords = pixelCoordsToWorldCoords(pixelCoords);
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
		if (event.isNotType(EventType_MouseButtonLeft)) {
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
		glm::vec3 WorldCoords = pixelCoordsToWorldCoords(pixelCoords);
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
			if (designerState == CompDesignState::SELECT)
			{
				deleteActivePrimitive();
			}
			break;

			// --------------------------------------------------------------------------------------------------------------- //
		}
	}
}

void ComponentDesigner::onMouseDragEvent(MouseDragEvent& event) 
{

	Base2DEngine::onMouseDragEvent(event);
	uint64_t eventID = event.ID;

	glm::vec2 pixelCoords = event.mousePosition;
	glm::vec3 WorldCoords = pixelCoordsToWorldCoords(pixelCoords);
	glm::vec2 screenCoords = { WorldCoords.x, WorldCoords.y };
	if (designerState == CompDesignState::SELECT) {
		//User is dragging a component.
		glm::vec2 translation = pixelDistanceToWorldDistance(event.currentFrameDelta);
		if (m_activeVertex) {
			//First check if we should move a vertex
			//We need to move a vertex
			if (m_activePoly) {
				m_activePoly->translateVertex(m_activeVertex, translation);
			}
			else if (m_activeLine) {
				m_activeLine->translateVertex(m_activeVertex, translation);
			}
		}
		else {
			//If we are not moving a vertex, then check to move primitives
			if (m_activePoly) {
				m_activePoly->translate(translation);
				m_lastDragPos = screenCoords;
			}
			if (m_activeLine) {
				m_activeLine->translate(translation);
				m_lastDragPos = getNearestGridVertex(screenCoords);
			}
			if (m_activeCircle) {
				m_activeCircle->translate(translation);
				m_lastDragPos = getNearestGridVertex(screenCoords);
			}
			if (m_activeText) {
				//Consideration: Should we keep track of the text position in the parent port/component? If so, this should be updated here.
				m_activeText->translate(translation);
				m_lastDragPos = getNearestGridVertex(screenCoords);
			}
			if (m_activePort.get()) {
				m_activePort->move(translation);
				m_lastDragPos = getNearestGridVertex(screenCoords);
			}
		}
	}
	std::string msg = "Delta: " + std::to_string(event.currentFrameDelta.x) + " + " + std::to_string(event.currentFrameDelta.y);
	LUMEN_LOG_DEBUG(msg, "Comp Design Drag");
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
		if (m_activePoly) {
			m_activePoly->translateTo(getNearestGridVertex(m_activePoly->m_trackedCenter));
		}
		if (m_activeLine) {
			m_activeLine->translateTo(getNearestGridVertex(m_activeLine->m_trackedCenter));
		}
		if (m_activeCircle) {
			m_activeCircle->translateTo(getNearestGridVertex(m_activeCircle->m_trackedCenter));
		}
		if (m_activeVertex) {
			if (m_activePoly) {
				m_activePoly->translateVertexTo(m_activeVertex, getNearestGridVertex(m_activeVertex->data.position));
			}
			else if (m_activeLine) {
				m_activeLine->translateVertexTo(m_activeVertex, getNearestGridVertex(m_activeVertex->data.position));
			}
		}
		if (m_activePort) {
			m_activePort->moveTo(getNearestGridVertex(m_activePort->centre));
		}
	}
}