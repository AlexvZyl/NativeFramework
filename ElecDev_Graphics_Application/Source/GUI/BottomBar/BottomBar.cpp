//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "BottomBar.h"
#include "Lumen.h"
#include "Application/Application.h"
#include "imgui/notify/imgui_notify.h"

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

BottomBar::BottomBar(std::string name, int windowFlags)
    : LumenWindow(name.c_str(), windowFlags)
{
    addImGuiWindowFlags( ImGuiWindowFlags_NoMove
                       | ImGuiWindowFlags_NoDecoration
                       | ImGuiWindowFlags_NoResize);
    m_defaultColor = ImGui::GetStyle().Colors[ImGuiCol_Separator];
    getNotificationColour();
}

void BottomBar::onImGuiBegin()
{
    getNotificationColour();
    ImGui::PushStyleColor(ImGuiCol_WindowBg, m_color);
    ImGui::PushStyleColor(ImGuiCol_Border, m_color);
    ImGui::BeginViewportSideBar(getImGuiName(), ImGui::GetMainViewport(), ImGuiDir_Down, 16.f, getImGuiWindowFlags());
}

void BottomBar::onImGuiRender()
{
    ImGui::SetCursorPos({8.f, 0.f});
    ImGui::Text("v0.1");
}

void BottomBar::onImGuiEnd()
{
    ImGui::End();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
}

void BottomBar::getNotificationColour() 
{
    static glm::vec4 blue = { 0.f, 0.f, 0.45f, 1.f };
    static glm::vec4 red = { 0.45f, 0.f, 0.f, 1.f };
    static glm::vec4 green = { 0.f, 0.45f, 0.f, 1.f };
    static glm::vec4 yellow = { 0.45f, 0.45f, 0.f, 1.f };

    // Default to blue.
    if (!ImGui::notifications.size())
    {
        m_color = m_defaultColor;
        return;
    }
    // Change colour based on notification.
    else 
    {
        switch (ImGui::notifications[0].get_type()) 
        {
        case ImGuiToastType_Info:
            m_color = blue;
            break;
        case ImGuiToastType_Success:
            m_color = green;
            break;
        case ImGuiToastType_Error:
            m_color = red;
            break;
        case ImGuiToastType_Warning:
            m_color = yellow;
            break;
        }
    }
}

//==============================================================================================================================================//
//  EOF.																																	    //
//==============================================================================================================================================//
