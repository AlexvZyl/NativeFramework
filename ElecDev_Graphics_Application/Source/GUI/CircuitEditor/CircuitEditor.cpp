/*=======================================================================================================================================*/
/* Includes.																															 */
/*=======================================================================================================================================*/

#include "CircuitEditor.h"
#include "Graphics/graphicsHandler.h"
#include "ImGui/misc/cpp/imgui_stdlib.h"
#include "GUI/guiHandler.h"

/*=======================================================================================================================================*/
/* Circuit Editor.																													 */
/*=======================================================================================================================================*/

CircuitEditor::CircuitEditor(GUIState* guiState, GraphicsHandler* graphicsHandler)
	: m_guiState(guiState), m_graphicsHandler(graphicsHandler), m_circuitName("Untitled " + std::to_string(m_circuitCount))
{
}

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

	
	ImGui::InputText("Circuit Name", &m_circuitName);
	if (ImGui::Button("Create")) 
	{ 
		m_graphicsHandler->m_addWindow = true;
		m_guiState->circuitEditor = false;
		m_graphicsHandler->m_newWindowTitle = m_circuitName; 
		m_circuitName = "Untitled " + std::to_string(++m_circuitCount);
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