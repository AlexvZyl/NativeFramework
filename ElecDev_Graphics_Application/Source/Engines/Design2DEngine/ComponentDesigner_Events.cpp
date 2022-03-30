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
			m_activePoly->addVertex(WorldCoords);
		}
		else if (designerState == CompDesignState::DRAW_LINE)
		{
			if (!m_activeLine) {
				//start new line
				m_activeLine = m_activeComponent->addLine(getNearestGridVertex(screenCoords), getNearestGridVertex(screenCoords));
			}
			else {
				//end the line
				m_activeLine->setEnd(getNearestGridVertex(screenCoords));
				m_activeLine = nullptr;
			}
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
		//m_activePoly->addVertex(WorldCoords);
	}
	else if (designerState == CompDesignState::DRAW_LINE)
	{
		if (m_activeLine) {
			//update the line end position
			m_activeLine->setEnd(getNearestGridVertex(screenCoords));
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
			designerState = CompDesignState::DRAW_POLY;
			m_activePoly = m_activeComponent->addPoly(std::vector<glm::vec2>());
			break;

		case GLFW_KEY_L:
			//Add new line
			designerState = CompDesignState::DRAW_LINE;
			m_activeLine = nullptr;
			break;

		case GLFW_KEY_C:
			//Add new line
			designerState = CompDesignState::DRAW_CIRCLE;
			m_activeCircle = nullptr;
			break;
			// --------------------------------------------------------------------------------------------------------------- //

		case GLFW_KEY_ESCAPE:
			designerState = CompDesignState::SELECT;
			m_activeLine = nullptr;
			m_activePoly = nullptr;
			m_activeCircle = nullptr;
			break;

			// --------------------------------------------------------------------------------------------------------------- //

		case GLFW_KEY_DELETE:
			break;

			// --------------------------------------------------------------------------------------------------------------- //
		}
	}
}