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
}

void Ribbon::begin()
{
    // Remove rounding so that it docks nicely.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2,2));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.f, 0.f, 0.04f, 1.00f));
    // Setup ribbon.
    ImGui::SetNextWindowBgAlpha(1);
    ImGui::Begin(m_name.c_str(), &m_isOpen, m_imguiWindowFlags);
}

void Ribbon::onRender()
{
    // -------------- //
    //  B U T T O N S //
    // -------------- //

    // Add design engine.
    if (ImGui::ImageButton((void*)m_componentIcon, ImVec2(30, 30), { 0, 1 }, { 1, 0 }))
    {
        
    }

    // Add design engine.
    if (ImGui::ImageButton((void*)m_circuitIcon, ImVec2(30, 30), { 0, 1 }, { 1, 0 }))
    {
        Lumen::getApp().pushGuiLayer<CircuitEditor>("Circuit Creator", DockPanel::Left);
    }
}

void Ribbon::end() 
{
    ImGui::End();
    // Pop Window rounding.
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
}

//==============================================================================================================================================//
//  EOF.																																	    //
//==============================================================================================================================================//