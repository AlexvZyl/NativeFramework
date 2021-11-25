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
    ImGui::SetNextWindowPos(m_guiState->popUpPosition);

    // Popup menu.
    if (ImGui::Begin("PopUpMenu", &m_guiState->popUpMenu, ImGuiWindowFlags_NoDecoration |
                                                          ImGuiWindowFlags_NoMove |
                                                          ImGuiWindowFlags_NoDocking |
                                                          ImGuiWindowFlags_AlwaysAutoResize))
    {
        // Close if not focused/
        if (!ImGui::IsWindowFocused()) {
            close();
        }
        // Render menu items.
        if (m_guiState->clickedZone.background) 
        {
            if (ImGui::MenuItem("Add component", "C"))
            {   
                close();
            }
        }
        else if (m_guiState->clickedZone.component) 
        {
            if (ImGui::MenuItem("Component Editor", "E"))
            {
                m_guiState->componentEditor = true;
                close();
            }
            if (ImGui::MenuItem("Edit Ports", "P"))
            {
                close();
            }
            if (ImGui::MenuItem("Remove component", "R"))
            {
                close();
            }
        }
        ImGui::End();
    }
}

void PopUpMenu::close()
{
    m_guiState->clickedZone.background = false;
    m_guiState->clickedZone.component = false;
    m_guiState->clickedZone.primative= false;
    m_guiState->clickedZone.port = false;
    m_guiState->popUpMenu = false;
}

/*=======================================================================================================================================*/
/* EOF.																																	 */
/*=======================================================================================================================================*/