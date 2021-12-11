/*=======================================================================================================================================*/
/* Includes.																															 */
/*=======================================================================================================================================*/

#include "PopUpMenu.h"
#include <iostream>
#include "Core/imgui.h"
#include "../GUIState.h"
#include "../Graphics/graphicsHandler.h"
#include "Utilities/Windows/WindowsUtilities.h"
#include "Utilities/Serialisation/Serialiser.h"
#include "CoreGL/EngineCoreGL.h"

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
        // Close if not focused.
        if (!ImGui::IsWindowFocused()) {
            close();
        }
        // Render menu items.
        if (m_guiState->clickedZone.background) 
        {
            if (ImGui::MenuItem("Place component", "P"))
            {   
                //get screen coordinates
                float pixelCoords[] = { m_guiState->renderWindowMouseCoordinate.x, m_guiState->renderWindowMouseCoordinate.y };
                glm::vec3 WorldCoords = m_guiState->design_engine->pixelCoordsToWorldCoords(pixelCoords);
                float screenCoords[2] = { WorldCoords[0], WorldCoords[1] };
                //place a dummy component
                m_guiState->design_engine->ComponentPlaceMode(screenCoords);
                close();
            }
        }
        else if (m_guiState->clickedZone.component) 
        {
            if (ImGui::MenuItem("Component Editor", "E"))
            {
                ImGui::SetNextWindowPos(m_guiState->popUpPosition);
                m_guiState->componentEditor = true;
                close();
            }
            /*if (ImGui::MenuItem("Edit Ports", "P"))
            {
                close();
            }*/
            if (ImGui::MenuItem("Add Cable", "C"))
            {
                close();
            }
            if (ImGui::MenuItem("Remove component", "DEL"))
            {
                close();
            }
        }
        ImGui::Separator();
        // Render the default items.
        if (ImGui::MenuItem("Load Circuit...", "Ctrl+L"))
        {
            //ShellExecuteA(NULL, "open", getExecutableLocation().c_str(), NULL, NULL, SW_SHOWDEFAULT);
            m_guiState->popUpMenu = false;
        }
        if (ImGui::MenuItem("Save Circuit...", "Ctrl+S"))
        {
            m_guiState->popUpMenu = false;
            std::string directory = selectFolder(getExecutableLocation());
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