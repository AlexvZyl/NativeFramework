/*=======================================================================================================================================*/
/* Includes.																															 */
/*=======================================================================================================================================*/

#include "GUI/guiHandler.h"
#include "CircuitEditor.h"
#include "Graphics/graphicsHandler.h"

/*=======================================================================================================================================*/
/* Component Editor.																													 */
/*=======================================================================================================================================*/

CircuitEditor::CircuitEditor(GUIState* guiState, GraphicsHandler* graphicsHandler)
	: m_guiState(guiState), m_graphicsHandler(graphicsHandler)
{}

void CircuitEditor::render() 
{
	// ----------- //
	//  S E T U P  //
	// ----------- //

	ImVec4 newCol = ImVec4(0.05f, 0.05f, 0.07f, 0.9f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, newCol);
	//FIX ME!! The wondow size should be set dynamically
	ImGui::SetNextWindowSize(ImVec2{ 400.f, 200.f }, ImGuiCond_Once);
	ImGui::Begin("Circuit Editor", &m_guiState->circuitEditor, ImGuiWindowFlags_NoDocking);

	// ----------------- //
	//  C O N T E N T S  //
	// ----------------- //

	static char circuitName[128] = "";
	ImGui::InputText("Circuit Name", &circuitName[0], 100);
	if (ImGui::Button("Create")) 
	{ 
		m_graphicsHandler->m_addWindow = true;
		m_guiState->circuitEditor = false;
		m_graphicsHandler->m_newWindowTitle = circuitName;
		memset(circuitName, 0, sizeof(circuitName));
	}

	// --------- //
	//  D O N E  //
	// --------- //

	ImGui::End();
	ImGui::PopStyleColor();
}

/*=======================================================================================================================================*/
/* EOF.																																	 */
/*=======================================================================================================================================*/