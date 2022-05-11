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
	if (m_componentOverwrite || m_cableOverwrite)
	{
		overWriteModal();
	}
	else if (m_deleteCables || m_deleteComponents) 
	{
		deleteModal();
	}
}

void CircuitDesignerPopupModal::overWriteModal() 
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
		auto* engine = Lumen::getApp().getActiveEngine<CircuitDesigner>();
		if (engine)
		{
			if (m_componentOverwrite)
			{
				engine->m_circuit->m_referenceComponents[m_entityPath.filename().string()] = YAML::LoadFile(m_entityPath.string())["Component"];
				if (m_mousePosition.x != -1.f)
					engine->loadAndPlaceComponent(m_entityPath, m_mousePosition);
			}
			else if (m_cableOverwrite)
			{
				YAML::Node node = YAML::LoadFile(m_entityPath.string());
				engine->m_circuit->m_referenceCables[m_entityPath.filename().string()] = node["Cable"];;
				if (m_mousePosition.x != -1.f)
					engine->loadDataToCable(node, engine->m_activeCable.get());
			}
		}
		closeWindow();
	}
	ImGui::SameLine();
	if (ImGui::Button("Cancel")) closeWindow();
}

void CircuitDesignerPopupModal::deleteModal() 
{
	std::string type;
	if (m_deleteCables) type = "cables";
	if (m_deleteComponents) type = "components";
	std::string msg = "You are about to delete " + std::to_string(m_entityCount) + " " + type + " along with the reference.";
	ImGui::Text(msg.c_str());
	ImGui::Text("Are you sure?");
	if (ImGui::Button("Delete"))
	{
		auto* engine = Lumen::getApp().getActiveEngine<CircuitDesigner>();
		if (m_deleteComponents) engine->removeImportedComponent(m_entity, false);
		else if (m_deleteCables) engine->removeImportedCable(m_entity, false);
		closeWindow();
	}
	ImGui::SameLine();
	if (ImGui::Button("Cancel"))
	{
		closeWindow();
	}
}