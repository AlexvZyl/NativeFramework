#include "CircuitDesignerPopupModal.h"
#include "Application/Application.h"
#include "Lumen.h"
#include "Engines/CircuitDesigner/CircuitDesigner.h"
#include "Engines/CircuitDesigner/Peripherals/Circuit.h"

CircuitDesignerPopupModal::CircuitDesignerPopupModal(const std::string& name, int imguiWindowFlags) 
	: PopupModal(name, imguiWindowFlags)
{}

void CircuitDesignerPopupModal::onImGuiRender() 
{
	std::string msg;
	if (m_componentOverwrite)
	{
		msg = "You are about to overwrite the reference component '" + m_entityPath.filename().string() + "'.";
	}
	else if (m_cableOverwrite) 
	{
		msg = "You are about to overwrite the reference cable '" + m_entityPath.filename().string() + "'.";
	} 
	
	ImGui::Text(msg.c_str()); 
	ImGui::Text("If you made changes to the file, previous versions may change when you reload the circuit.");
	ImGui::Text("Are you sure?");

	if (ImGui::Button("Overwrite"))
	{
		if (m_componentOverwrite)
		{
			auto* engine = Lumen::getApp().getActiveEngine<CircuitDesigner>();
			if (engine)
			{
				engine->m_circuit->m_uniqueComponents[m_entityPath.filename().string()] = YAML::LoadFile(m_entityPath.string())["Component"];
				if(m_mousePosition.x != -1.f)
					engine->loadAndPlaceComponent(m_entityPath, m_mousePosition);
			}
		}
		else if (m_cableOverwrite)
		{

		}
		closeWindow();
	}
	ImGui::SameLine();
	if (ImGui::Button("Cancel")) closeWindow();
}