//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Utilities/WebSocket/LumenWebSocket.h"
#include "SettingsWidget.h"
#include "Application/Application.h"
#include "GLFW/glfw3.h"

//==============================================================================================================================================//
//  Popup menu.																																	//
//==============================================================================================================================================//

SettingsWidget::SettingsWidget(std::string name, int imguiWindowFlags)
	: GuiElementCore(name, imguiWindowFlags)
{}

void SettingsWidget::begin()
{
	ImGui::SetNextWindowSize(glm::vec2(350, 200), ImGuiCond_Once);
	ImGui::Begin(m_name.c_str(), &m_isOpen, m_imguiWindowFlags);
}

void SettingsWidget::onRender()
{
    Application& app = Lumen::getApp();

    // Wait or Poll GLFW events.
    ImGui::Checkbox("  Wait Events", &app.m_waitForEvents);
    static int timeoutFPS = (int)(1.f / app.m_eventsTimeout);
    // Timeout slider.
    if (app.m_waitForEvents)
    {
        ImGui::Text("Idle: ");
        ImGui::SameLine();
        if (ImGui::SliderInt("##EventsTimeout", &timeoutFPS, 10, app.m_targetFPS, "%.0f", ImGuiSliderFlags_AlwaysClamp))
        {
            app.m_eventsTimeout = 1.f / timeoutFPS;
        }
    }

    ImGui::Separator();

    // FPS cap.
    static int fps = (int)app.m_targetFPS;
    ImGui::Text("FPS:");
    ImGui::SameLine();
    if (ImGui::SliderInt("##TargetFPS", &fps, 10, 144, "%d", ImGuiSliderFlags_AlwaysClamp))
    {
        // Scale timeout FPS with FPS if at max.
        if (app.m_targetFPS == timeoutFPS)
            timeoutFPS = fps;

        // Update fps.
        app.m_targetFPS = fps;
        app.m_targetFrameTime = 1 / app.m_targetFPS;

        // Reduce timeout fps if larger than target.
        if (timeoutFPS > fps)
            timeoutFPS == fps;
    }

    ImGui::Separator();

    // Window decorations.
    static bool windowDecorations = true;
    if (ImGui::Checkbox("  Window Decorations", &windowDecorations))
        glfwSetWindowAttrib(app.getWindow(), GLFW_DECORATED, windowDecorations);

    ImGui::Separator();

    // Display websocket information.
    std::string websocketInfo = "Websocket:  ";
    websocketInfo += "ws://";
    websocketInfo += app.m_webSocket->m_socketAddress.to_string() + ":" + std::to_string(app.m_webSocket->m_port);
    ImGui::Text(websocketInfo.c_str());
}

void SettingsWidget::end()
{
	ImGui::End();
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//