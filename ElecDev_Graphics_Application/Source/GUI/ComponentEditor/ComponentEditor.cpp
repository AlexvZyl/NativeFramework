/*=======================================================================================================================================*/
/* Includes.																															 */
/*=======================================================================================================================================*/

#include "GUI/guiHandler.h"
#include "ComponentEditor.h"

/*=======================================================================================================================================*/
/* Component Editor.																													 */
/*=======================================================================================================================================*/

ComponentEditor::ComponentEditor(GUIState* guiState, GraphicsHandler* graphicsHandler)
	: m_guiState(guiState), m_graphicsHandler(graphicsHandler)
{}

void ComponentEditor::render() 
{
	// Place editor at correct position.
	ImGui::SetNextWindowPos(m_guiState->popUpPosition);
	// Editor menu.
	ImVec4 newCol = ImVec4(0.05f, 0.05f, 0.07f, 0.9f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, newCol);
	ImGui::Begin("Comoponent Editor", &m_guiState->componentEditor, ImGuiWindowFlags_NoDocking);
	//ImGui::Button("New MCC");
	//ImGui::CollapsingHeader("Ports");
	if (ImGui::TreeNode("Ports")) {
		if (ImGui::TreeNodeEx("Left")) {

			ImGui::TreePop();
		}	
		if (ImGui::TreeNode("Right")) {

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Top")) {

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Bottom")) {

			ImGui::TreePop();
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