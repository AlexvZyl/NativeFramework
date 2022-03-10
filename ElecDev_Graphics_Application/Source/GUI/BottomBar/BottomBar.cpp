//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "BottomBar.h"
#include "Lumen.h"
#include "Application/Application.h"

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

BottomBar::BottomBar(std::string name, int windowFlags)
    : GuiElementCore(name, windowFlags)
{
    m_imguiWindowFlags |= ImGuiWindowFlags_NoMove
                       | ImGuiWindowFlags_NoDecoration
                       | ImGuiWindowFlags_NoResize;
    m_color = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];
    m_color /= 1.5f;
    m_color.a = 1.f;
}

void BottomBar::begin()
{
    ImGui::PushStyleColor(ImGuiCol_WindowBg, m_color);
    ImGui::PushStyleColor(ImGuiCol_Border, m_color);
    ImGui::BeginViewportSideBar(m_name.c_str(), ImGui::GetMainViewport(), ImGuiDir_Down, 16.f, m_imguiWindowFlags);
}

void BottomBar::onRender()
{
    ImGui::SetCursorPos({8.f, 0.f});
    ImGui::Text("v0.0.1");
}

void BottomBar::end()
{
    ImGui::End();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
}

//==============================================================================================================================================//
//  EOF.																																	    //
//==============================================================================================================================================//