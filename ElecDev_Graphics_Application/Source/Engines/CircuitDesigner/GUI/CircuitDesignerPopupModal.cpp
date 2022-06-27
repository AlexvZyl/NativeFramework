#include "CircuitDesignerPopupModal.h"
#include "Application/Application.h"
#include "Lumen/Lumen.h"
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
	if (!m_yamlNode)
	{
		if (m_componentOverwrite)	msg = "You are about to overwrite the reference component '" + m_entityPath.filename().string() + "'.";
		else if (m_cableOverwrite)	msg = "You are about to overwrite the reference cable '" + m_entityPath.filename().string() + "'.";
	}
	else if (m_yamlNode)
	{
		if (m_componentOverwrite)	msg = "You are about to overwrite the reference component '" + m_entity + "'.";
		else if (m_cableOverwrite)	msg = "You are about to overwrite the reference cable '" + m_entity + "'.";
	}

	ImGui::Text(msg.c_str());
	ImGui::Text("If you made changes to the file, previous versions may change when you reload the circuit.");
	ImGui::Text("Are you sure?");

	if (ImGui::Button("Overwrite"))
	{
		auto* engine = Lumen::getApp().getActiveEngine<CircuitDesigner>();
		if (engine)
		{
			// --------- //
			//  F I L E  //
			// --------- //

			if (!m_yamlNode)
			{
				if (m_componentOverwrite)
				{
					YAML::Node compNode = YAML::LoadFile(m_entityPath.string())["Component"];
					engine->m_circuit->m_referenceComponents[m_entityPath.filename().string()] = compNode;
					if (m_mousePosition.x != -1.f) engine->loadAndPlaceComponent(m_entityPath, m_mousePosition);
					engine->overwriteComponents(m_entityPath.filename().string(), compNode);
				}
				else if (m_cableOverwrite)
				{
					YAML::Node node = YAML::LoadFile(m_entityPath.string())["Cable"];
					engine->m_circuit->m_referenceCables[m_entityPath.filename().string()] = node;
					if (m_mousePosition.x != -1.f) engine->loadDataToCable(node, engine->m_activeCable.get());
					engine->overwriteCables(m_entityPath.filename().string(), node);
				}
			}

			// ------------------- //
			//  Y A M L   N O D E  //
			// ------------------- //

			else if (m_yamlNode)
			{
				if (m_componentOverwrite)
				{
					engine->m_circuit->m_referenceComponents[m_entity] = m_node;
					if (m_mousePosition.x != -1.f) engine->loadAndPlaceComponent(m_node, m_mousePosition);
					engine->overwriteComponents(m_entity, m_node);
				}
				else if (m_cableOverwrite)
				{
					engine->m_circuit->m_referenceCables[m_entity] = m_node;;
					if (m_mousePosition.x != -1.f) engine->loadDataToCable(m_node, engine->m_activeCable.get());
					engine->overwriteCables(m_entity, m_node);
				}
			}
		}
		closeWindow();
	}
	ImGui::SameLine();
	if (ImGui::Button("Cancel")) closeWindow();
}

void CircuitDesignerPopupModal::deleteModal()
{
	std::string msg;
	if (m_deleteCables)     msg = "You are about to clear " + std::to_string(m_entityCount) + " cables and delete the reference.";
	if (m_deleteComponents) msg = "You are about to delete " + std::to_string(m_entityCount) + " components along with the reference.";
	ImGui::Text(msg.c_str());
	ImGui::Text("Are you sure?");
	if (ImGui::Button("Continue"))
	{
		auto* engine = Lumen::getApp().getActiveEngine<CircuitDesigner>();
		if (m_deleteComponents)  engine->removeImportedComponent(m_entity, false);
		else if (m_deleteCables) engine->removeImportedCable(m_entity, false);
		closeWindow();
	}
	ImGui::SameLine();

	if (ImGui::Button("Cancel")) closeWindow();
}