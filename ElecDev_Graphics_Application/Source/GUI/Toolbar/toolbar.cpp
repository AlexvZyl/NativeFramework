/*=======================================================================================================================================*/
/* Includes                                                                                                                              */
/*=======================================================================================================================================*/

#include <iostream>
#include <cmath>
#include <cfenv>
#include "toolbar.h"
#include "Utilities/Windows/WindowsUtilities.h"
#include "Lumen.h"
#include "GUI/RendererStats/RendererStats.h"
#include "Application/Application.h"
#include "Resources/ResourceHandler.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "GUI/ImGuiDemoWindow/ImGuiDebugWindow.h"
#include <GLFW/glfw3.h>

/*=======================================================================================================================================*/
/* Constructor.                                                                                                                          */
/*=======================================================================================================================================*/

// Constructor.
Toolbar::Toolbar(std::string& name, int windowFlags)
    : GuiElementCore(name, windowFlags)
{
    // Load texture 1. 
    static BITMAP textureBM = loadImageFromResource(ICON_PNG);
    m_texWidth = textureBM.bmWidth;
    m_texHeight = textureBM.bmHeight;
    m_texID = loadBitmapToGL(textureBM);
}

/*=======================================================================================================================================*/
/* Rendering                                                                                                                             */
/*=======================================================================================================================================*/

void Toolbar::begin() 
{
    // Style.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.05f, 0.05f, 0.05f, 1.00f));
    // Begin.
    m_isOpen = ImGui::BeginMainMenuBar();
}

void Toolbar::onRender()
{
    // Get the app.
    Application& app = Lumen::getApp();
    
    // Draw the image.
    static float textureSize = ImGui::GetFont()->FontSize + 2 * TOOLBAR_PADDING - 5;
    ImGui::Image((void*)m_texID, ImVec2(textureSize, textureSize), ImVec2(0, 1), ImVec2(1, 0));

    // --------- //
    //  F I L E  //
    // --------- //

    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Load...", "Ctrl+O"))
        {
            // Create a load event.
            std::string path = selectFile("Lumen Load Circuit", "", "", "Load");
            FileLoadEvent event(path);
            app.logEvent<FileLoadEvent>(event);
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Close", "Ctrl+W"))
        {
            Lumen::getApp().closeWindow();
        }

        ImGui::EndMenu();
    }

    // --------- //
    //  E D I T  //
    // --------- //

    if (ImGui::BeginMenu("Edit"))
    {
        if (ImGui::MenuItem("Undo", "CTRL+Z", false, false))
        {

        }
        if (ImGui::MenuItem("Redo", "CTRL+Y", false, false))
        {

        }

        ImGui::Separator();

        if (ImGui::MenuItem("Cut", "CTRL+X", false, false))
        {

        }
        if (ImGui::MenuItem("Copy", "CTRL+C", false, false))
        {

        }
        if (ImGui::MenuItem("Paste", "CTRL+V", false, false))
        {

        }
        ImGui::EndMenu();
    }

    // ----------- //
    //  T O O L S  //
    // ----------- //

    if(ImGui::BeginMenu("Tools"))
    {
        static bool stats = false;
        if (ImGui::Checkbox("Renderer Info", &stats))
        {
            static std::string statsName;
            if (stats)
            {
                auto* layer = Lumen::getApp().pushGuiLayer<RendererStats>("Renderer Info", DockPanel::Right, 0, false);
                statsName = layer->getName();
            }
            else 
            {
                Lumen::getApp().queuePopLayer(statsName);
            }
        }   
        
        ImGui::Separator();
       
        // Style editor.
        static bool style = false;
        if (ImGui::Checkbox("Style Editor", &style)) 
        {
            static std::string styleLayernName;
            if (style)
            {
                auto* layer = Lumen::getApp().pushGuiLayer<ImGuiDebugWindow>("Style Editor", DockPanel::Floating, 0, false);
                layer->getGui()->showStyleEditor = true;
                styleLayernName = layer->getName();
            }
            else
            {
                Lumen::getApp().queuePopLayer(styleLayernName);
            }
        }

        // ImGui demo window.
        static bool demo = false;
        if (ImGui::Checkbox("Demo Window", &demo))
        {
            static std::string demoLayerName;
            if (demo)
            {
                auto* layer = Lumen::getApp().pushGuiLayer<ImGuiDebugWindow>("Demo Window", DockPanel::Floating, 0, false);
                layer->getGui()->showDemoWindow = true;
                demoLayerName = layer->getName();
            }
            else
            {
                Lumen::getApp().queuePopLayer(demoLayerName);
            }
        }
        ImGui::EndMenu();
    }
};

void Toolbar::end()
{
    // End.
    ImGui::EndMainMenuBar();
    // Style.
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
}

/*=======================================================================================================================================*/
/* EOF.                                                                                                                                  */
/*=======================================================================================================================================*/