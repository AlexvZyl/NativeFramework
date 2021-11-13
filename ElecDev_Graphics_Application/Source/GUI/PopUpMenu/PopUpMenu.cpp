/*=======================================================================================================================================*/
/* Includes.																															 */
/*=======================================================================================================================================*/

#include "PopUpMenu.h"
#include <iostream>
#include "Core/imgui.h"
#include "../GUIState.h"
#include "../Graphics/graphicsHandler.h"

/*=======================================================================================================================================*/
/* PopUp Menu.																															 */
/*=======================================================================================================================================*/

// Constructor.
PopUpMenu::PopUpMenu(GUIState* guiState, GraphicsHandler* graphicsHandler)
    : m_guiState(guiState), m_graphicsHandler(graphicsHandler)
{}

// Render call.
void PopUpMenu::render() 
{
    ImVec2 parentPos = ImGui::GetWindowPos();
    ImGui::SetNextWindowPos(ImVec2(m_guiState->popUpPosition.x+parentPos.x, 
                                   m_guiState->popUpPosition.y + parentPos.y));
    ImGui::Begin("PopUpMenu", &m_guiState->popUpMenu, ImGuiWindowFlags_NoDecoration |
                                                      ImGuiWindowFlags_NoMove       |
                                                      ImGuiWindowFlags_NoDocking    |
                                                      ImGuiWindowFlags_NoFocusOnAppearing);
    if (ImGui::MenuItem("Add component", "C")) { /* Do stuff */ }
    if (ImGui::MenuItem("Add port", "P")) { /* Do stuff */ }
    ImGui::Separator();
    if (ImGui::MenuItem("Remove component", "R")) { /* Do stuff */ }
    ImGui::End();
}

/*=======================================================================================================================================*/
/* EOF.																																	 */
/*=======================================================================================================================================*/