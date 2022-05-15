//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "SceneHierarchy.h"
#include "Lumen.h"
#include "Application/Application.h"
#include "Engines/CircuitDesigner/CircuitDesigner.h"
#include "Engines/CircuitDesigner/Peripherals/Component2D.h"
#include "Engines/CircuitDesigner/Peripherals/Cable.h"
#include "Engines/CircuitDesigner/Peripherals/Circuit.h"
#include "Application/ApplicationTemplates.h"

//==============================================================================================================================================//
//  Popup menu.																																	//
//==============================================================================================================================================//

SceneHierarchy::SceneHierarchy(std::string name, int imguiWindowFlags)
	: LumenWindow(name, imguiWindowFlags)
{}

void SceneHierarchy::onImGuiBegin()
{
	ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 10.f);
	ImGui::Begin(getImGuiName(), NULL, getImGuiWindowFlags());
}

void SceneHierarchy::onImGuiRender()
{
	Application& app = Lumen::getApp();

	// Check for active engine.
	CircuitDesigner* engine = app.getActiveEngine<CircuitDesigner>();
	if (!engine)
	{
		ImGui::Text("No active engine.");
		return;
	}
	
	ImGui::Separator();

	// Display components.
	if (ImGui::CollapsingHeader("Components"))
	{
		int compCount = 0;
		int portCount = 0;
		ImGui::Indent(GUI_INDENT_WIDTH);
		for (auto& component : engine->m_circuit->m_components)
		{
			ImGui::PushID(compCount++);
			if (ImGui::CollapsingHeader(component->titleString.c_str()))
			{
				// Set active button.
				if (ImGui::Button("Set Active"))
					engine->setActiveComponent(component->m_entityID);

				// Setup ports table.
				ImGui::BeginTable("Ports", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchProp);
				ImGui::TableSetupColumn("Port", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableSetupColumn("Cable", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableHeadersRow();

				// Ports.
				for (auto& port : component->ports)
				{
					ImGui::PushID(portCount++);
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(port->m_label.c_str());
					ImGui::TableSetColumnIndex(1);
					if(port->m_cables.size())
						ImGui::Text(port->m_cables[0]->m_titleString.c_str());
					ImGui::PopID();		
				}
				ImGui::EndTable();
			}
			ImGui::PopID();
		}
		ImGui::Unindent(GUI_INDENT_WIDTH);
	}

	ImGui::Separator();

	// Display cables.
	if (ImGui::CollapsingHeader("Cables"))
	{
		ImGui::Indent(GUI_INDENT_WIDTH);
		int cableCount = 0;
		for (auto& cable : engine->m_circuit->m_cables)
		{
			ImGui::PushID(cableCount++);
			if (ImGui::CollapsingHeader(cable->m_titleString.c_str()))
			{
				if (ImGui::Button("Set Active"))
				{
					engine->m_activeComponent = nullptr;
					engine->setActiveCable(cable->m_entityID);
				}
			}
			ImGui::PopID();
		}
		ImGui::Unindent(GUI_INDENT_WIDTH);
	}

	ImGui::Separator();

}

void SceneHierarchy::onImGuiEnd()
{
	ImGui::End();
	ImGui::PopStyleVar();
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//