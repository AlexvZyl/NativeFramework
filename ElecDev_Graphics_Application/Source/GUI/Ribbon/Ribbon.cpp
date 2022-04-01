//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Ribbon.h"
#include <imgui/imgui.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <cfenv>
#define STB_IMAGE_IMPLEMENTATION
#include <External/Misc/stb_image.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui_internal.h>
#include "GUI/ComponentEditor/ComponentEditor.h"

#include "Lumen.h"
#include "Application/Application.h"
#include "Engines/Design2DEngine/Design2DEngine.h"
#include "GUI/CircuitEditor/CircuitEditor.h"
#include "Utilities/Windows/WindowsUtilities.h"
#include "Engines/Design2DEngine/ComponentDesigner.h"
#include "GUI/SettingsWidget/SettingsWidget.h"

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

// Constructor.
Ribbon::Ribbon(std::string name, int windowFlags)
    : GuiElementCore(name, windowFlags)
{
    m_imguiWindowFlags  |=  ImGuiWindowFlags_NoMove
                        |   ImGuiWindowFlags_NoDecoration
                        |   ImGuiWindowFlags_NoResize;        

    m_circuitIcon = loadBitmapToGL(loadImageFromResource(CIRCUIT_FILE_ICON));
    m_componentIcon = loadBitmapToGL(loadImageFromResource(COMPONENT_FILE_ICON));
    m_loadFileIcon = loadBitmapToGL(loadImageFromResource(LOAD_FILE_ICON));
    m_settingsIcon = loadBitmapToGL(loadImageFromResource(SETTINGS_ICON));
    m_userIcon = loadBitmapToGL(loadImageFromResource(USER_ICON));
}

void Ribbon::begin()
{
    // Remove rounding so that it docks nicely.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2,2));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.f, 0.f, 0.04f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_Button, { 0.f, 0.f, 0.f, 0.f });
    // Setup ribbon.
    ImGui::SetNextWindowBgAlpha(1);
    ImGui::Begin(m_name.c_str(), &m_isOpen, m_imguiWindowFlags);
}

void Ribbon::onRender()
{
    Application& app = Lumen::getApp();
    static glm::vec2 buttonSize(30,30); 

    // ------------------- //
    //  L O A D   F I L E  //
    // ------------------- //

    // Button.
    if (ImGui::ImageButton((void*)m_loadFileIcon, buttonSize, { 0, 1 }, { 1, 0 }))
    {
        // Create and log load event.
        std::string path = selectFile("Lumen Load Circuit", "", "", "Load");
        if (path.size())
        {
            FileLoadEvent event(path);
            app.logEvent<FileLoadEvent>(event);
        }
    }
    // Tooltip.
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::Text("Open");
        ImGui::EndTooltip();
    }

    ImGui::Separator();

    // ----------------------------------- //
    //  C O M P O N E N T   B U I L D E R  //
    // ----------------------------------- //

    // Button.
    if (ImGui::ImageButton((void*)m_componentIcon, buttonSize, { 0, 1 }, { 1, 0 }))
    {
        Lumen::getApp().pushEngineLayer<ComponentDesigner>("Component Designer");
    }
    // Tooltip.
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::Text("Component designer");
        ImGui::EndTooltip();
    }

    // --------------------------------- //
    //  C I R C U I T   D E S I G N E R  //
    // --------------------------------- //

    // Button.
    if (ImGui::ImageButton((void*)m_circuitIcon, buttonSize, { 0, 1 }, { 1, 0 }))
    {
        Lumen::getApp().pushGuiLayer<CircuitEditor>("Circuit Editor", DockPanel::Left);
    }
    // Tooltip.
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::Text("Circuit Designer");
        ImGui::EndTooltip();
    }
    
    ImGui::Separator();

    ////
    ////  NB: THIS SECTION RENDERS FROM THE BOTTOM UP!  Still needs some work...
    ////
    ImGuiStyle& style = ImGui::GetStyle();
    float cursorPosY = 0;
    
    // ----------------- //
    //  S E T T I N G S  //
    // ----------------- //

    cursorPosY = m_contentRegionSize.y - (buttonSize.y + style.FramePadding.y * 4);
    ImGui::SetCursorPosY(cursorPosY);
    // Button.
    if (ImGui::ImageButton((void*)m_settingsIcon, buttonSize, { 0, 1 }, { 1, 0 }))
    {
        app.pushGuiLayer<SettingsWidget>("Settings", DockPanel::Floating);
    }
    // Tooltip.
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::Text("Settings");
        ImGui::EndTooltip();
    }

    // --------------- //
    //  P R O F I L E  //
    // --------------- //

    cursorPosY -= (buttonSize.y + style.FramePadding.y * 2 + style.ItemSpacing.y * 2);
    ImGui::SetCursorPosY(cursorPosY);
    // Button.
    if (ImGui::ImageButton((void*)m_userIcon, buttonSize, { 0, 1 }, { 1, 0 }))
    {

    }
    // Tooltip.
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::Text("Profile");
        ImGui::EndTooltip();
    }
}

void Ribbon::end() 
{
    ImGui::End();
    // Pop Window rounding.
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
}

//==============================================================================================================================================//
//  EOF.																																	    //
//==============================================================================================================================================//