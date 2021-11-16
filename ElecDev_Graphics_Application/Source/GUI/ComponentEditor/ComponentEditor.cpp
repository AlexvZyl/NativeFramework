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
	/*colors[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.05f, 0.07f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);*/

	// Place editor at correct position.
	ImGui::SetNextWindowPos(m_guiState->popUpPosition, ImGuiCond_Once);
	// Editor menu.
	ImVec4 newCol = ImVec4(0.05f, 0.05, 0.07f, 0.9f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, newCol);
	ImGui::Begin("Comoponent Editor", &m_guiState->componentEditor, ImGuiWindowFlags_NoDocking);
	ImGui::Button("New MCC");
	ImGui::Separator();
	ImGui::End();
	ImGui::PopStyleColor();
}

/*=======================================================================================================================================*/
/* EOF.																																	 */
/*=======================================================================================================================================*/