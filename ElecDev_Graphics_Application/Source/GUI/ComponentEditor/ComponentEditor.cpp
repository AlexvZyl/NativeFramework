/*=======================================================================================================================================*/
/* Includes.																															 */
/*=======================================================================================================================================*/
#pragma once
//#include "GUI/guiHandler.h"
#include "ComponentEditor.h"
#include "Design2DEngineGL/Peripherals/Component2D.h"
#include "Design2DEngineGL/Peripherals/Port.h"
#include "ImGui/misc/cpp/imgui_stdlib.h"

/*=======================================================================================================================================*/
/* Component Editor.																													 */
/*=======================================================================================================================================*/

ComponentEditor::ComponentEditor(GUIState* guiState, GraphicsHandler* graphicsHandler)
	: m_guiState(guiState), m_graphicsHandler(graphicsHandler)
{}

void ComponentEditor::render() 
{
	//If we are not in a design engine, we should not be here.
	//We need to implement a check for this.

	//	Fetch The active component.
	Component2D* activeComponent = m_guiState->active_component;
	// Place editor at correct position.
	//ImGui::SetNextWindowPos(m_guiState->popUpPosition);
	// Editor menu.
	ImVec4 newCol = ImVec4(0.05f, 0.05f, 0.07f, 0.9f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, newCol);
	//FIX ME!! The wondow size should be set dynamically
	ImGui::SetNextWindowSize(ImVec2 {400.f, 200.f}, ImGuiCond_Once);
	ImGui::Begin("Comoponent Editor", &m_guiState->componentEditor, ImGuiWindowFlags_NoDocking);
	//ImGui::Button("New MCC");
	//ImGui::CollapsingHeader("Ports");
	ImGui::Text("Component Name:");
	ImGui::SameLine();
	if (ImGui::InputText("##ComponentName", &activeComponent->titleString)) {
		activeComponent->title->updateText(activeComponent->titleString);
	}

	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::TreeNode("Ports")) {
		ImGui::BeginTable("Current ports", 4, ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingFixedFit);
			//Setup table
			ImGui::TableSetupColumn("Location    ");
			ImGui::TableSetupColumn("Port Name");
			ImGui::TableSetupColumn("I/O Type       ");
			ImGui::TableHeadersRow();

			ImGui::TableNextRow();
			std::vector < std::vector<std::shared_ptr<Port>>> allPorts = {	activeComponent->portsWest, 
																			activeComponent->portsEast,
																			activeComponent->portsNorth,
																			activeComponent->portsSouth};
			std::vector<std::string> portPositions = { "Left", "Right", "Top", "Bottom" };

			for (int i = 0; i < allPorts.size(); i++) {
				std::vector<std::shared_ptr<Port>> portsSide = allPorts[i];
				int j = 0;
				for (std::shared_ptr<Port> port : portsSide) {
					//Set labels
					char labelName[20];
					sprintf_s(labelName, "##N%d,%d", i, j);
					char labelPos[20];
					sprintf_s(labelPos, "##P%d,%d", i, j);
					char labelType[20];
					sprintf_s(labelType, "##T%d,%d", i, j);
					char labelRemove[20];
					sprintf_s(labelRemove, "Remove##%d,%d", i, j++);


					//create a table entry for the port
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					//Add the position
					ImGui::PushItemWidth(-1);
					ImGui::Text(portPositions[i].c_str());
					//ImGui::Combo(labelPos, &selection, "Left\0Right\0Top\0Bottom");
					ImGui::PopItemWidth();
					ImGui::TableNextColumn();
					//Add the Name
					ImGui::PushItemWidth(-1);
					if(ImGui::InputText(labelName, &port->m_label)) {
						port->title->updateText(port->m_label);
					}
					ImGui::PopItemWidth();
					ImGui::TableNextColumn();
					//Add the type
					ImGui::PushItemWidth(-1);
					int* typeval = (int*)&port->m_type;
					ImGui::Combo(labelType, typeval, "IN\0OUT\0IN/OUT");
					ImGui::PopItemWidth();
					ImGui::TableNextColumn();
					//Add a "remove" button
					if (ImGui::Button(labelRemove)) {
						activeComponent->removePort(port);
					}
				}
				if (j) ImGui::Separator();
			}
			if (addingPort) {
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
				if (ImGui::Button("Confirm")) {
					//Add the port to the component.
					activeComponent->addPort(newPos, (PortType) newType, newName);
					addingPort = false;
				}
			}
			ImGui::EndTable();
			if (!addingPort) {
				if (ImGui::Button("New Port")) {
					addingPort = true;
				}
			}
		ImGui::TreePop();
	}
	//ImGui::Separator();
	ImGui::End();
	ImGui::PopStyleColor();
}

/*=======================================================================================================================================*/
/* EOF.																																	 */
/*=======================================================================================================================================*/