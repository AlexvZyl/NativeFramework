/*=======================================================================================================================================*/
/* Includes                                                                                                                             */
/*=======================================================================================================================================*/

#include <Core/imgui.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <cfenv>
#include "toolbar.h"

/*=======================================================================================================================================*/
/* Functions                                                                                                                             */
/*=======================================================================================================================================*/

//static void ShowExampleMenuFile();
//
//static void HelpMarker(const char* desc)
//{
//    ImGui::TextDisabled("(?)");
//    if (ImGui::IsItemHovered())
//    {
//        ImGui::BeginTooltip();
//        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
//        ImGui::TextUnformatted(desc);
//        ImGui::PopTextWrapPos();
//        ImGui::EndTooltip();
//    }
//}

void renderToolbar()
{
    // hmmmmmmmm...  
    //ImGuiWindowFlags_ noMove = 1;
    //ImGui::Begin("Tooblar", ImGuiWindowFlags_NoMove= noMove);
    /*ImGui::Begin("Toolbar");
    ImGui::Button("MEL");
    ImGui::Button("LV CircuitDB");
    ImGui::Button("MV CircuitDB");
    ImGui::Button("MV LoadList");
    ImGui::Button("LV LoadList");
    ImGui::Button("Cable Schedule");
    ImGui::Button("Bill of Quantities");
    ImGui::End();
    ImGui::Begin("Expanded Toolbar");
    ImGui::End();
    ImGui::Begin("Scene");
    ImGui::End();*/\

    static bool show_app_main_menu_bar = false;
    static bool show_app_documents = false;

    static bool show_app_console = false;
    static bool show_app_log = false;
    static bool show_app_layout = false;
    static bool show_app_property_editor = false;
    static bool show_app_long_text = false;
    static bool show_app_auto_resize = false;
    static bool show_app_constrained_resize = false;
    static bool show_app_simple_overlay = false;
    static bool show_app_fullscreen = false;
    static bool show_app_window_titles = false;
    static bool show_app_custom_rendering = false;

    static bool show_app_metrics = false;
    static bool show_app_style_editor = false;
    static bool show_app_about = false;

    static bool no_titlebar = false;
    static bool no_scrollbar = false;
    static bool no_menu = false;
    static bool no_move = false;
    static bool no_resize = false;
    static bool no_collapse = false;
    static bool no_close = false;
    static bool no_nav = false;
    static bool no_background = false;
    static bool no_bring_to_front = false;
    static bool unsaved_document = false;
    static bool my_tool_active = true;

    // Menu
    ImVec2 size = ImVec2(32.0f, 32.0f);
    ImGui::Begin("Toolbar", &my_tool_active, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::SetWindowPos(ImVec2(0, 0));
    if (ImGui::BeginMenuBar())
    {
        
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
            if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
            if (ImGui::MenuItem("Close", "Ctrl+W")) { my_tool_active = false; }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("meh"))
        {
            if (ImGui::MenuItem("aa..", "Ctrl+O")) { /* Do stuff */ }
            if (ImGui::MenuItem("Sasve", "Ctrl+S")) { /* Do stuff */ }
            if (ImGui::MenuItem("Claose", "Ctrl+W")) { my_tool_active = false; }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    ImGui::End();
    
    
    
    
    
}

