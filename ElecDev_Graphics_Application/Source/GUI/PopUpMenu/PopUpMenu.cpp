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
PopUpMenu::PopUpMenu(GUIState* guiState)
    : m_guiState(guiState)
{}

// Render call.
void PopUpMenu::render()
{
    // Place at mouse position.
    ImVec2 parentPos = ImGui::GetWindowPos();
    ImGui::SetNextWindowPos(ImVec2(m_guiState->popUpPosition.x + parentPos.x,
                                   m_guiState->popUpPosition.y + parentPos.y));

    // Popup menu.
    if (ImGui::Begin("PopUpMenu", &m_guiState->popUpMenu, ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoDocking)) 
    {
        // Close if not focused/
        m_guiState->popUpMenu = ImGui::IsWindowFocused();
        // Render menu items.
        if (ImGui::MenuItem("Add component", "C")) { /* Do stuff */ }
        if (ImGui::MenuItem("Component Editor", "E"))
        {
            m_guiState->popUpMenu = false;
            m_guiState->componentEditor = true;
        }
        if (ImGui::MenuItem("Add port", "P", false, false)) { /* Do stuff */ }
        ImGui::Separator();
        if (ImGui::MenuItem("Remove component", "R")) { /* Do stuff */ }
        ImGui::End();
    }
}

/*=======================================================================================================================================*/
/* EOF.																																	 */
/*=======================================================================================================================================*/