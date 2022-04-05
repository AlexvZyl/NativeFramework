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

		if (designerState == CompDesignState::DRAW_POLY)
		{

			if (!m_activePoly)
			{
				m_activePoly = m_activeComponent->addPoly({ getNearestGridVertex(screenCoords), getNearestGridVertex(screenCoords) });
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
			designerState = CompDesignState::DRAW_POLY;
			m_activePoly = nullptr;
			break;

		case GLFW_KEY_L:
			//Add new line
			designerState = CompDesignState::DRAW_LINE;
			m_activeLine = nullptr;
			break;

		case GLFW_KEY_C:
			//Add new circle
			designerState = CompDesignState::DRAW_CIRCLE;
			m_activeCircle = nullptr;
			break;

		case GLFW_KEY_O:
			//Add new port
			designerState = CompDesignState::DRAW_CIRCLE;
			m_activeCircle = nullptr;
			break;
			// --------------------------------------------------------------------------------------------------------------- //

		case GLFW_KEY_ESCAPE:
			if (designerState == CompDesignState::DRAW_CIRCLE || designerState == CompDesignState::DRAW_POLY || designerState == CompDesignState::DRAW_LINE) {
				if (m_activeCircle) {
					Renderer::remove(m_activeCircle);
				}
				if (m_activePoly) {
					Renderer::remove(m_activePoly);
				}
				if (m_activeLine) {
					Renderer::remove(m_activeLine);
				}
			}
			m_activeLine = nullptr;
			m_activePoly = nullptr;
			m_activeCircle = nullptr;
			designerState = CompDesignState::SELECT;
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