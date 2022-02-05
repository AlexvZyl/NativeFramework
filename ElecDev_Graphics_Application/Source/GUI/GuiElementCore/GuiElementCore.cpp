//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "GUI/GuiElementCore/GuiElementCore.h"
#include "Application/Events/Events.h"
#include "External/ImGUI/Core/imgui.h"

//==============================================================================================================================================//
//  Rendering.																																	//
//==============================================================================================================================================//

GuiElementCore::GuiElementCore(std::string name, int windowFlags)
	: m_name(name), m_imguiWindowFlags(windowFlags)
{}

void GuiElementCore::begin() 
{
	ImGui::Begin(m_name.c_str(), &m_isOpen, m_imguiWindowFlags);
}

void GuiElementCore::renderBody() 
{
	// Override this function to render the window.
}

void GuiElementCore::end()
{
	ImGui::End();
}

//==============================================================================================================================================//
//  On Event.																																	//
//==============================================================================================================================================//

void GuiElementCore::onEvent(Event& event) 
{
	if (event.isConsumed()) return;

	uint64_t eventID = event.getID();

	if (eventID == EventType_WindowResize) onWindowResizeEvent(dynamic_cast<WindowResizeEvent&>(event));

	// Currently GUI events are handles in the GLFW callback init.
	// It uses the ImGUI GLFW implementation code.  This means that 
	// we do not need to pass any inputs to the GUI element.

	event.consume();
}

//==============================================================================================================================================//
//  Window Events.																																//
//==============================================================================================================================================//

void GuiElementCore::onWindowResizeEvent(WindowResizeEvent& event) 
{
	glm::vec2 windowSize = event.getWindowSize();
	m_size = { windowSize.x, windowSize.y };
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//