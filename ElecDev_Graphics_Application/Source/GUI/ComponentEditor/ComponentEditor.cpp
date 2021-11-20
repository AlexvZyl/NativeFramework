/*=======================================================================================================================================*/
/* Includes.																															 */
/*=======================================================================================================================================*/
#pragma once
//#include "GUI/guiHandler.h"
#include "ComponentEditor.h"
#include "Design2DEngineGL/Peripherals/Component2D.h"

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

	// Place editor at correct position.
	ImGui::SetNextWindowPos(m_guiState->popUpPosition);
	// Editor menu.
	ImVec4 newCol = ImVec4(0.05f, 0.05f, 0.07f, 0.9f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, newCol);
	ImGui::Begin("Comoponent Editor", &m_guiState->componentEditor, ImGuiWindowFlags_NoDocking);
	//ImGui::Button("New MCC");
	//ImGui::CollapsingHeader("Ports");
	
	//	Fetch The active component.
	Component2D* activeComponent = m_guiState->active_component;

	if (ImGui::TreeNode("Ports")) {
		if (ImGui::BeginTable("Current ports", 4)) {
			//Setup table
			ImGui::TableSetupColumn("Name");
			ImGui::TableSetupColumn("Location");
			ImGui::TableHeadersRow();

			ImGui::TableNextRow();
			for (std::shared_ptr<Port> port : activeComponent->portsWest) {
				ImGui::TableNextColumn();
				char input[20];
				strcpy_s(input, port->label.c_str());
				ImGui::InputText("##N1", input, 20);
				//ImGui::Text(port->label.c_str());
				ImGui::TableNextColumn();
				int selection = 0;
				ImGui::Combo("##P0", &selection, "Left\0Right\0Top\0Bottom");
				ImGui::TableNextRow();
			}

			for (std::shared_ptr<Port> port : activeComponent->portsEast) {
				ImGui::TableNextColumn();
				char input[20];
				strcpy_s(input, port->label.c_str());
				ImGui::InputText("##N1", input, 20);
				//ImGui::Text(port->label.c_str());
				ImGui::TableNextColumn();
				int selection = 1;
				ImGui::Combo("##P1", &selection, "Left\0Right\0Top\0Bottom");
				ImGui::TableNextRow();
			}

			for (std::shared_ptr<Port> port : activeComponent->portsNorth) {
				ImGui::TableNextColumn();
				char input[20];
				strcpy_s(input, port->label.c_str());
				ImGui::InputText("##N1", input, 20);
				//ImGui::Text(port->label.c_str());
				ImGui::TableNextColumn();
				int selection = 2;
				ImGui::Combo("##P2", &selection, "Left\0Right\0Top\0Bottom");
				ImGui::TableNextRow();
			}

			for (std::shared_ptr<Port> port : activeComponent->portsSouth) {
				ImGui::TableNextColumn();
				char input[20];
				strcpy_s(input, port->label.c_str());
				ImGui::InputText("##N1", input, 20);
				//ImGui::Text(port->label.c_str());
				ImGui::TableNextColumn();
				int selection = 3;
				ImGui::Combo("##P3", &selection, "Left\0Right\0Top\0Bottom");
				ImGui::TableNextRow();
			}
			ImGui::EndTable();
		}
		ImGui::TreePop();
	}
	ImGui::Separator();
	ImGui::End();
	ImGui::PopStyleColor();
}

/*=======================================================================================================================================*/
/* EOF.																																	 */
/*=======================================================================================================================================*/