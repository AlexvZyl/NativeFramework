#include "ComponentDesigner.h"
#include "Application/Events/Events.h"
#include "GUI/PopUpMenu/PopUpMenu.h"
#include "Lumen.h"
#include "Application/Application.h"

void ComponentDesigner::onMouseButtonEvent(MouseButtonEvent& event)
{
	Base2DEngine::onMouseButtonEvent(event);
	uint64_t eventID = event.ID;

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
}

void ComponentDesigner::onMouseScrollEvent(MouseScrollEvent& event)
{
	Base2DEngine::onMouseScrollEvent(event);
}

void ComponentDesigner::onKeyEvent(KeyEvent& event)
{
	Base2DEngine::onKeyEvent(event);
}