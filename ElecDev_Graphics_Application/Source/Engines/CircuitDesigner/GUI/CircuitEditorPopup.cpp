#include "Engines/CircuitDesigner/CircuitDesigner.h"
#include "CircuitEditorPopup.h"
#include "Application/Application.h"
#include "Lumen/Lumen.h"

CircuitEditorPopup::CircuitEditorPopup(const std::string& name, int imguiWindowFlags) 
	: LumenPopupWindow(name, imguiWindowFlags)
{}

void CircuitEditorPopup::onImGuiRender() 
{
	Application& app = Lumen::getApp();
	CircuitDesigner* engine = app.getActiveEngine<CircuitDesigner>();

	if (!engine) closeWindow();

	if (ImGui::MenuItem("Delete"))
	{
		if (m_component)  engine->removeImportedComponent(m_entity);
		else if (m_cable) engine->removeImportedCable(m_entity);
		closeWindow();
	}
}

void CircuitEditorPopup::setCable(const std::string& cable) 
{
	m_entity = cable;
	m_cable = true;
	m_component = false;
}

void CircuitEditorPopup::setComponent(const std::string& component)
{
	m_entity = component;
	m_cable = false;
	m_component = true;
}