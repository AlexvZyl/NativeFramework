/*=======================================================================================================================================*/
/* Includes.																															 */
/*=======================================================================================================================================*/

#include "PopUpMenu.h"
#include <iostream>
#include "Core/imgui.h"
#include "../GUIState.h"
#include "Graphics/graphicsHandler.h"
#include "Utilities/Windows/WindowsUtilities.h"
#include "Utilities/Serialisation/Serialiser.h"
#include "Graphics/OpenGL/Design2DEngineGL/Design2D_Engine.h"
#include "Graphics/OpenGL/Design2DEngineGL/Peripherals/Circuit.h"

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
    // Place at mouse position.
    ImGui::SetNextWindowPos(m_guiState->popUpPosition);

    // Popup menu.
    if (ImGui::Begin("PopUpMenu", &m_guiState->popUpMenu, ImGuiWindowFlags_NoDecoration |
                                                          ImGuiWindowFlags_NoMove |
                                                          ImGuiWindowFlags_NoDocking |
                                                          ImGuiWindowFlags_AlwaysAutoResize))
    {
        if (!m_contextSaved)
        {
            // Save context.
            m_windowContext = m_graphicsHandler->m_activeWindow->windowName;
            m_contextSaved = true;
        }

        // Set active window.
        m_graphicsHandler->m_activeWindow = m_graphicsHandler->m_windowsDictionary[m_windowContext];

        // Close if not focused.
        if (!ImGui::IsWindowFocused()) 
        { close(); }

        // Render menu items.
        if (m_guiState->clickedZone.background) 
        {
            if (ImGui::MenuItem("Place component", "P"))
            {   
                //get screen coordinates
                float pixelCoords[] = { m_guiState->renderWindowMouseCoordinate.x, m_guiState->renderWindowMouseCoordinate.y };
                glm::vec3 WorldCoords = m_guiState->design_engine->pixelCoordsToWorldCoords(pixelCoords);
                glm::vec2 screenCoords = { WorldCoords.x, WorldCoords.y };
                //place a dummy component
                Design2DEngineGL* activeEngine = reinterpret_cast<Design2DEngineGL*>(m_graphicsHandler->m_windowsDictionary[m_windowContext]->engineGL.get());
                activeEngine->ComponentPlaceMode(screenCoords);
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
                m_guiState->design_engine->deleteActiveComponent();
                close();
            }
        }
        ImGui::Separator();

        // --------------- //
        //  D E F A U L T  //
        // --------------- //

        if (ImGui::MenuItem("Load Circuit...", "Ctrl+L"))
        {
            m_graphicsHandler->m_loadEvent.eventTrigger = true;
            m_graphicsHandler->m_loadEvent.path = selectFile();
            close();
        }
        if (ImGui::MenuItem("Save Circuit...", "Ctrl+S"))
        {
            m_graphicsHandler->m_saveEvent.eventTrigger = true;
            m_graphicsHandler->m_saveEvent.saveEngine = m_windowContext;
            Design2DEngineGL* activeEngine = reinterpret_cast<Design2DEngineGL*>(m_graphicsHandler->m_activeWindow->engineGL.get());
            m_graphicsHandler->m_saveEvent.path = selectFile("", activeEngine->m_circuit->m_label, "Save");
            close();
        }
        ImGui::End();
    }
    // On component close.
    if(!m_guiState->popUpMenu)
    { 
        close();
    }
}

void PopUpMenu::close()
{
    m_guiState->clickedZone.background = false;
    m_guiState->clickedZone.component = false;
    m_guiState->clickedZone.primative= false;
    m_guiState->clickedZone.port = false;
    m_guiState->popUpMenu = false;
    m_contextSaved = false;
}

/*=======================================================================================================================================*/
/* EOF.																																	 */
/*=======================================================================================================================================*/