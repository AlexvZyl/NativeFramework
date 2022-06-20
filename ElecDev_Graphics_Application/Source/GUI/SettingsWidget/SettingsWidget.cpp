//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "boost/beast/core.hpp"
#include "boost/beast/websocket.hpp"
#include "Utilities/WebSocket/LumenWebSocket.h"
#include "SettingsWidget.h"
#include "Application/Application.h"
#include "Engines/EngineCore/EngineCore.h"
#include "OpenGL/Primitives/Grid.h"
#include "OpenGL/Primitives/Grid.h"
#include "OpenGL/SceneGL.h"
#include "GLFW/glfw3.h"
#include "GUI/GraphicsScene/GraphicsScene.h"
#include "Engines/EngineCore/EngineCore.h"

//==============================================================================================================================================//
//  Popup menu.																																	//
//==============================================================================================================================================//

SettingsWidget::SettingsWidget(std::string name, int imguiWindowFlags)
	: LumenWindow(name, imguiWindowFlags)
{
    addImGuiWindowFlags(ImGuiWindowFlags_AlwaysAutoResize);
}

void SettingsWidget::onImGuiBegin()
{
	ImGui::SetNextWindowSize(glm::vec2(350, 200), ImGuiCond_Once);
	ImGui::Begin(getImGuiName(), &m_isOpen, getImGuiWindowFlags());
}

void SettingsWidget::onImGuiRender()
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
    if (ImGui::SliderInt("##TargetFPS", &fps, 10, 144, "%d"))
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
        glfwSetWindowAttrib(app.getGLFWWindow(), GLFW_DECORATED, windowDecorations);

    ImGui::Separator();

    // Display websocket information.
    std::string websocketInfo = "Websocket:  ";
    websocketInfo += "ws://";
    websocketInfo += app.m_webSocket->m_socketAddress->to_string() + ":" + std::to_string(app.m_webSocket->m_port);
    ImGui::Text(websocketInfo.c_str());

    // Edit scene grid settings.
    EngineCore* engine = app.getActiveEngine();
    static int newGridCount = 10;
    if (engine) 
    {
        Grid& grid = engine->getScene().getGrid();
        ImGui::Separator();
        int gridLines = grid.getTotalCoarseLines();
        std::string gridCount = "Scene current grid lines: " + std::to_string(gridLines);
        ImGui::Text(gridCount.c_str());
        ImGui::Text("New Grid Count: ");
        ImGui::SameLine();
        ImGui::SliderInt("##SceneGridLines", &newGridCount, 1, 100);
        if (ImGui::Button("Update Grid"))
        {
            grid.destroyGrid().setTotalCoarseLines(newGridCount).createGrid();
        }

        ImGui::Text("Grid Scale:");

        float sliderValue = grid.getScale();
        ImGui::SliderFloat("##GridScaleSlider", &sliderValue, 1.f/10, 1.f*10);
        grid.setScale(sliderValue);

        ImGui::Separator();
    }

    // Anti Aliasing.
    ImGui::Separator();
    const char* AA = { "None\0MSAA1\0MSAA2\0MSAA4\0MSAA8\0MSAA16\0MSAA32\0" };
    static int currentItem = 3;
    ImGui::Text("Anti-Alisaing");
    ImGui::SameLine();
    ImGui::Combo("##AA", &currentItem, AA);
    if (ImGui::Button("Apply"))
    {
        // Change MSAA value for attachments.
        FrameBufferSamples newSamples;
        switch (currentItem)
        {
        case 1:  newSamples = FrameBufferSamples::MSAA1; break;
        case 2:  newSamples = FrameBufferSamples::MSAA2; break;
        case 3:  newSamples = FrameBufferSamples::MSAA4; break;
        case 4:  newSamples = FrameBufferSamples::MSAA8; break;
        case 5:  newSamples = FrameBufferSamples::MSAA16; break;
        case 6:  newSamples = FrameBufferSamples::MSAA32; break;
        default: newSamples = FrameBufferSamples::NORMAL; break;
        }

        // Update value renderer uses.
        Renderer::MSAA = newSamples;

        // Iterate engines.
        for (auto* engine : Lumen::getApp().getEnignes())
        {
            // Update attachments in the FBO's.
            FrameBufferObject& fbo = engine->getScene().m_msaaFBO;
            for (auto& [slot, attachment] : fbo.getAttachments())
            {
                attachment.samples = newSamples;
            }
            // Recreate if using MSAA.
            if (fbo.isOnGPU() && newSamples != FrameBufferSamples::NORMAL)
            {
                fbo.recreate();
            }
            // Create if switched to MSAA.
            else if (!fbo.isOnGPU() && newSamples != FrameBufferSamples::NORMAL)
            {
                fbo.create();
            }
            // Destroy MSAA FBO if no longer using MSAA.
            else if (fbo.isOnGPU() && newSamples == FrameBufferSamples::NORMAL)
            {
                fbo.destroy();
            }
        }
    }
    ImGui::Text("Note: MSAA1 still uses a multi-sampled framebuffer.");
}

void SettingsWidget::onImGuiEnd()
{
	ImGui::End();
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//