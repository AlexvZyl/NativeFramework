/*=======================================================================================================================================*/
/* Includes                                                                                                                              */
/*=======================================================================================================================================*/

#include <Core/imgui.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <cfenv>
#include "toolbar.h"
#include "Utilities/Windows/WindowsUtilities.h"
#include "Graphics/graphicsHandler.h"

/*=======================================================================================================================================*/
/* Constructor.                                                                                                                          */
/*=======================================================================================================================================*/

// Constructor.
Toolbar::Toolbar(GUIState* guiState, GraphicsHandler* graphicsHandler)
    : m_guiState(guiState), m_graphicsHandler(graphicsHandler)
{
    this->my_tool_active = true;
    this->show_app_main_menu_bar = false;
    this->show_app_documents = false;

    this->show_app_console = false;
    this->show_app_log = false;
    this->show_app_layout = false;
    this->show_app_property_editor = false;
    this->show_app_long_text = false;
    this->show_app_auto_resize = false;
    this->show_app_constrained_resize = false;
    this->show_app_simple_overlay = false;
    this->show_app_fullscreen = false;
    this->show_app_window_titles = false;
    this->show_app_custom_rendering = false;

    this->show_app_metrics = false;
    this->show_app_style_editor = false;
    this->show_app_about = false;

    this->no_titlebar = false;
    this->no_scrollbar = false;
    this->no_menu = false;
    this->no_move = false;
    this->no_resize = false;
    this->no_collapse = false;
    this->no_close = false;
    this->no_nav = false;
    this->no_background = false;
    this->no_bring_to_front = false;
    this->unsaved_document = false;
    this->my_tool_active = true;
};

/*=======================================================================================================================================*/
/* Functions                                                                                                                             */
/*=======================================================================================================================================*/

void Toolbar::renderToolbar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open..", "Ctrl+O")) 
            {
                m_graphicsHandler->m_loadEvent.eventTrigger = true;
                m_graphicsHandler->m_loadEvent.path = selectFile();
            }
            if (ImGui::MenuItem("Close", "Ctrl+W")) { m_guiState->globalQuit = true; }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
};

/*=======================================================================================================================================*/
/* EOF.                                                                                                                                  */
/*=======================================================================================================================================*/