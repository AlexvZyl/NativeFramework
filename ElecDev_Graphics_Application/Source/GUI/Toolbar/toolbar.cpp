/*=======================================================================================================================================*/
/* Includes                                                                                                                             */
/*=======================================================================================================================================*/

#include <Core/imgui.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <cfenv>
#include "toolbar.h"
#include "../Helper/stateMachine.h"


// Constructor.
Toolbar::Toolbar(stateMachine states)
{
    this->states = states;
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
    ImVec2 mainWindow = ImGui::GetMainViewport()->WorkSize;
    Toolbar:
    ImGui::Begin("Toolbar", &this->my_tool_active, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    ImGui::SetWindowSize(ImVec2(mainWindow.x+1, 30));
    ImGui::SetWindowPos(ImVec2(-1, 0));
    
    if (ImGui::BeginMenuBar())
    {

        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
            if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
            if (ImGui::MenuItem("Close", "Ctrl+W")) { exit(1); }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Test 1")) { /* Do stuff */ }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    ImGui::End();

};