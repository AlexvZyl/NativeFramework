#pragma once

/*=======================================================================================================================================*/
/* Includes.																															 */
/*=======================================================================================================================================*/

#include "imgui/imgui.h"
#include "ComponentEditor.h"
#include "ImGui/misc/cpp/imgui_stdlib.h"
#include "OpenGL/RendererGL.h"
#include "Engines/Design2DEngine/Peripherals/Component2D.h"
#include "Engines/Design2DEngine/Peripherals/Port.h"
#include "GUI/GuiElementCore/GuiElementCore.h"
#include "Application/Application.h"	

/*=======================================================================================================================================*/
/* Component Editor.																													 */
/*=======================================================================================================================================*/

ComponentEditor::ComponentEditor(std::string name, int windowFlags)
	: GuiElementCore(name, windowFlags)
{

}

void ComponentEditor::begin() 
{
	// Place editor at correct position.
	/*ImGui::SetNextWindowPos(m_guiState->popUpPosition);*/
	// FIX ME!! The wondow size should be set dynamically
	ImGui::SetNextWindowSize(ImVec2{ 465.f, 400 }, ImGuiCond_Once);
	ImGui::Begin(m_name.c_str(), &m_isOpen, m_imguiWindowFlags);
}

void ComponentEditor::onRender()
{
	//	Fetch The active component.
	Component2D* activeComponent = Lumen::getApp().m_guiState->active_component;
	//check that the active component exists. Close if not.
	if (!activeComponent)
	{
		Lumen::getApp().m_guiState->componentEditor = false;
		return;
	}

	ImVec4 newCol = ImVec4(0.05f, 0.05f, 0.07f, 0.9f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, newCol);
	ImGui::SameLine();
	ImGui::SameLine();
	if (ImGui::InputText("##ComponentName", &activeComponent->titleString))
	{
		activeComponent->title->updateText(activeComponent->titleString);
	}

	ImGui::InputText("##Equipment Type", &activeComponent->equipType);

	// Get Active component type to change component editor based on type

	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::TreeNode("Ports"))
	{
		ImGui::BeginTable("Current ports", 4, ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingFixedFit);
		//ImGui::SetColumnWidth(1, 20.f);

			//Setup table
		ImGui::TableSetupColumn("Location    ");
		ImGui::TableSetupColumn("Port Name");
		ImGui::TableSetupColumn("I/O Type       ");
		ImGui::TableHeadersRow();

		ImGui::TableNextRow();
		std::vector<std::vector<std::shared_ptr<Port>>> allPorts = { activeComponent->portsWest,
																	activeComponent->portsEast,
																	activeComponent->portsNorth,
																	activeComponent->portsSouth };

		std::vector<std::string> portPositions = { "Left", "Right", "Top", "Bottom" };

		for (int i = 0; i < allPorts.size(); i++)
		{
			std::vector<std::shared_ptr<Port>> portsSide = allPorts[i];
			int j = 0;
			for (std::shared_ptr<Port> port : portsSide)
			{
				// Table labels.
				char labelName[20];
				sprintf_s(labelName, "##N%d,%d", i, j);
				char labelPos[20];
				sprintf_s(labelPos, "##P%d,%d", i, j);
				char labelType[20];
				sprintf_s(labelType, "##T%d,%d", i, j);
				char labelRemove[20];
				sprintf_s(labelRemove, "Remove##%d,%d", i, j++);

				// Port entry in table.
				ImGui::TableNextRow();
				ImGui::TableNextColumn();

				// Position.
				ImGui::PushItemWidth(-1);
				ImGui::Text(portPositions[i].c_str());
				ImGui::PopItemWidth();
				ImGui::TableNextColumn();

				// Name.
				ImGui::PushItemWidth(185.f);
				if (ImGui::InputText(labelName, &port->m_label))
				{
					port->title->updateText(port->m_label);
				}
				ImGui::PopItemWidth();
				ImGui::TableNextColumn();

				// Type.
				ImGui::PushItemWidth(-1);
				int* typeval = (int*)&port->m_type;
				ImGui::Combo(labelType, typeval, "IN\0OUT\0IN/OUT");
				ImGui::PopItemWidth();
				ImGui::TableNextColumn();

				// Remove.
				if (ImGui::Button(labelRemove)) { activeComponent->removePort(port); }

			}
			if (j) ImGui::Separator();
		}

		if (addingPort) 
		{
			//create a table entry for the port to be added
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			//Add the position
			ImGui::PushItemWidth(-1);
			ImGui::Combo("##newPos", &newPos, "Left\0Right\0Top\0Bottom");
			ImGui::PopItemWidth();
			ImGui::TableNextColumn();
			//Add the Name
			ImGui::PushItemWidth(-1);
			ImGui::InputText("##newName", &newName);
			ImGui::PopItemWidth();
			ImGui::TableNextColumn();
			//Add the type
			ImGui::PushItemWidth(-1);
			ImGui::Combo("##newType", &newType, "IN\0OUT\0IN/OUT");
			ImGui::PopItemWidth();
			ImGui::TableNextColumn();
			//Add a "Confirm" button
			if (ImGui::Button("Confirm"))
			{
				// Add the port to the component.
				activeComponent->addPort(newPos, (PortType)newType, newName);
				addingPort = false;
			}
		}
		ImGui::EndTable();
		if (!addingPort)
		{
			if (ImGui::Button("New Port"))
			{
				addingPort = true;
			}
		}
		ImGui::TreePop();
	}


	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::TreeNode("Data Automation"))
	{
		ImGui::BeginTable("Columns to specify", 21, ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollX);

		//Setup table columns
		for (auto& [key, val]: activeComponent->cableDict) {
			ImGui::TableSetupColumn(key.c_str());
		}
		
		ImGui::TableHeadersRow();

		// Port entry in table.
		ImGui::TableNextRow();
		ImGui::TableNextColumn();

		for (auto& [key, val]: activeComponent->cableDict) {
			ImGui::PushItemWidth(-1);
			ImGui::InputText(key.c_str(), &val);

			ImGui::PopItemWidth();
			ImGui::TableNextColumn();

		}

		//ImGui::SetColumnWidth(1, 20.f);
		ImGui::EndTable();
		ImGui::TreePop();
	}

	ImGui::PopStyleColor();
}

void ComponentEditor::end()
{
	ImGui::End();
}

/*=======================================================================================================================================*/
/* EOF.																																	 */
/*=======================================================================================================================================*/