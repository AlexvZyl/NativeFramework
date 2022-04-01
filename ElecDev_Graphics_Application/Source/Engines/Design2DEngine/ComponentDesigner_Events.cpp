#include "ComponentDesigner.h"
#include "Application/Events/Events.h"
#include "GUI/PopUpMenu/PopUpMenu.h"
#include "Lumen.h"
#include "Application/Application.h"
#include "External/GLFW/Includes/GLFW/glfw3.h"
#include "Graphics/OpenGL/Primitives/Polygon.h"
#include "Graphics/OpenGL/Primitives/LineSegment.h"
#include "Graphics/OpenGL/Primitives/Circle.h"

void ComponentDesigner::onMouseButtonEvent(MouseButtonEvent& event)
{
	Base2DEngine::onMouseButtonEvent(event);
	uint64_t eventID = event.ID;


	if (eventID == (EventType_MousePress | EventType_MouseButtonLeft))
	{
		glm::vec2 pixelCoords = event.mousePosition;
		glm::vec3 WorldCoords = m_scene->pixelCoordsToWorldCoords(pixelCoords);
		glm::vec2 screenCoords = { WorldCoords.x, WorldCoords.y };

		if (designerState == CompDesignState::DRAW_POLY)
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
			if (!m_activeCircle) {
				//start new line
				m_activeCircle = Renderer::addCircle2D(getNearestGridVertex(screenCoords), 0.f, m_activeComponent->shapeColour, 1.0f, 0.f, m_activeComponent.get());
			}
			else {
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
	if (eventID == (EventType_MousePress | EventType_MouseButtonRight))
	{

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

void ComponentDesigner::onMouseMoveEvent(MouseMoveEvent& event)
{
	Base2DEngine::onMouseMoveEvent(event);

	glm::vec2 pixelCoords = event.mousePosition;
	glm::vec3 WorldCoords = m_scene->pixelCoordsToWorldCoords(pixelCoords);
	glm::vec2 screenCoords = { WorldCoords.x, WorldCoords.y };

	if (designerState == CompDesignState::DRAW_POLY)
	{
		//Move the back vertex
		if (m_activePoly) {
			m_activePoly->translateToVertexAtIndex(m_activePoly->m_vertexCount-1, getNearestGridVertex(screenCoords));
		}
	}
	else if (designerState == CompDesignState::DRAW_LINE)
	{
		if (m_activeLine) {
			//update the line end position
			m_activeLine->setEnd(getNearestGridVertex(screenCoords));
		}
	}
	else if (designerState == CompDesignState::DRAW_CIRCLE)
	{
		if (m_activeCircle) {
			//update circle
			m_activeCircle->setRadius(glm::length(glm::vec2(m_activeCircle->m_trackedCenter) - getNearestGridVertex(screenCoords)));
		}
	}
	else if (designerState == CompDesignState::PLACE_PORT)
	{
		if (m_activePort) {
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

}

void ComponentDesigner::onMouseScrollEvent(MouseScrollEvent& event)
{
	Base2DEngine::onMouseScrollEvent(event);
}

void ComponentDesigner::onKeyEvent(KeyEvent& event)
{
	Base2DEngine::onKeyEvent(event);

	// Get the event ID.
	uint64_t eventID = event.ID;

	// Events based on key type.
	if (eventID == EventType_KeyPress)
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