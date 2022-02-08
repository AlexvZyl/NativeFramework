/*=======================================================================================================================================*/
/* Includes                                                                                                                              */
/*=======================================================================================================================================*/

#include <Core/imgui.h>
#include <iostream>
#include <cmath>
#include <cfenv>
#include "toolbar.h"
#include "Utilities/Windows/WindowsUtilities.h"
#include <GLFW/glfw3.h>

/*=======================================================================================================================================*/
/* Constructor.                                                                                                                          */
/*=======================================================================================================================================*/

// Constructor.
Toolbar::Toolbar(std::string& name, int windowFlags)
    : GuiElementCore(name, windowFlags)
{}

/*=======================================================================================================================================*/
/* Rendering                                                                                                                             */
/*=======================================================================================================================================*/

void Toolbar::begin() 
{
    m_isClosed != ImGui::BeginMainMenuBar();
}

void Toolbar::renderBody()
{
    // Should not render if closed or collapsed.
    if (m_isCollapsed || m_isClosed) return;

    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Load..", "Ctrl+O"))
        {
            /*m_graphicsHandler->m_loadEvent.eventTrigger = true;
            m_graphicsHandler->m_loadEvent.path = selectFile("Lumen Load Circuit", "", "", "Load");*/
        }
        if (ImGui::MenuItem("Close", "Ctrl+W"))
        {
            /*m_guiState->globalQuit = true; */
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Edit"))
    {
        if (ImGui::MenuItem("Undo", "CTRL+Z"))
        {

        }
        if (ImGui::MenuItem("Redo", "CTRL+Y", false, false))
        {

        }

        ImGui::Separator();

        if (ImGui::MenuItem("Cut", "CTRL+X"))
        {

        }
        if (ImGui::MenuItem("Copy", "CTRL+C"))
        {

        }
        if (ImGui::MenuItem("Paste", "CTRL+V"))
        {

        }

        ImGui::EndMenu();
    }
};

void Toolbar::end()
{
    ImGui::EndMainMenuBar();
}

/*=======================================================================================================================================*/
/* EOF.                                                                                                                                  */
/*=======================================================================================================================================*/