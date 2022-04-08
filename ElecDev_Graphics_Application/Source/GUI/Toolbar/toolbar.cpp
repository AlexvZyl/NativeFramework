/*=======================================================================================================================================*/
/* Includes                                                                                                                              */
/*=======================================================================================================================================*/

#include <iostream>
#include <cmath>
#include <cfenv>
#include "toolbar.h"
#include "Utilities/Windows/WindowsUtilities.h"
#include "Lumen.h"
#include "Application/Application.h"
#include "Resources/ResourceHandler.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "GUI/RendererStats/RendererStats.h"
#include "GUI/AssetExplorer/AssetExplorer.h"
#include "GUI/ImGuiDemoWindow/ImGuiDebugWindow.h"
#include "GUI/SceneHierarchy/SceneHierarchy.h"
#include "GUI/BackgroundColorEditor/BackgroundColorEditor.h"
#include <GLFW/glfw3.h>
#include "Application/Events/EventLog.h"

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
    m_colour = ImGui::GetStyle().Colors[ImGuiCol_Separator];
}

/*=======================================================================================================================================*/
/* Rendering                                                                                                                             */
/*=======================================================================================================================================*/

void Toolbar::begin()
{
    // Style.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {0.f, TOOLBAR_PADDING});
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {14.f, 14.f});
    ImGui::PushStyleColor(ImGuiCol_MenuBarBg, m_colour);
    // Begin.
    m_isOpen = ImGui::BeginMainMenuBar();
}

void Toolbar::onRender()
{
    static glm::vec2 dropdownItemSpacing = {4.f, 4.f};

    // Get the app.
    Application& app = Lumen::getApp();
    
    // Draw the image.
    static float textureSize = 2 * TOOLBAR_PADDING + ImGui::GetFont()->FontSize - 11.f;
    ImGui::SetCursorPosY(m_contentRegionPosition.y+6);
    ImGui::Image((void*)m_texID, ImVec2(textureSize, textureSize), ImVec2(0, 1), ImVec2(1, 0));
    ImGui::SetCursorPosY(m_contentRegionPosition.y);

    // --------- //
    //  F I L E  //
    // --------- //

    if (ImGui::BeginMenu("File"))
    {
        // Load file.
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, dropdownItemSpacing);
        if (ImGui::MenuItem("Load...", "Ctrl+O"))
        {
            auto fsPath = selectFile("Lumen Load Circuit", "", "", "Load");
            if (fsPath.string().size())
            {
                EventLog::log<FileLoadEvent>(FileLoadEvent(fsPath.string(), EventType_Application));
            }
        }

        ImGui::Separator();

        // Close.
        if (ImGui::MenuItem("Close", "Ctrl+W"))
            Lumen::getApp().stopRunning();

        ImGui::EndMenu();
        ImGui::PopStyleVar();
    }

    // --------- //
    //  E D I T  //
    // --------- //

    if (ImGui::BeginMenu("Edit"))
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, dropdownItemSpacing);

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

        ImGui::PopStyleVar();
    }

    // ----------- //
    //  T O O L S  //
    // ----------- //

    if(ImGui::BeginMenu("Tools"))
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, dropdownItemSpacing);

        // Renderer info.
        static bool profilerState = false;
        if (ImGui::Checkbox(" Renderer Info", &profilerState))
        {
            static std::string statsName;
            if (profilerState)
            {
                auto* layer = app.pushGuiLayer<RendererStats>("Renderer Info", LumenDockPanel::Right, 0, false);
                statsName = layer->getName();
                app.m_profilerActive = true;
            }
            else 
            {
                app.m_profilerActive = false;
                app.queuePopLayer(statsName);
            }
        }   

        // Scene hierarchy.
        static bool sceneHierarchy = false;
        if (ImGui::Checkbox(" Scene Hierarchy", &sceneHierarchy))
        {
            static std::string sceneHierarchyname;
            if (sceneHierarchy)
            {
                auto* layer = app.pushGuiLayer<SceneHierarchy>("Scene Hierarchy", LumenDockPanel::Right, 0, false);
                sceneHierarchyname = layer->getName();
            }
            else
            {
                app.queuePopLayer(sceneHierarchyname);
            }
        }

        // Scene hierarchy.
        static bool assetExplorer = true;
        if (ImGui::Checkbox(" Asset Explorer", &assetExplorer))
        {
            static std::string assetExplorerName;
            if (assetExplorer)
            {
                m_assetExplorerLayer = app.pushGuiLayer<AssetExplorer>("Asset Explorer", LumenDockPanel::Bottom, 0, false);
            }
            else
            {
                app.queuePopLayer(m_assetExplorerLayer);
                m_assetExplorerLayer = nullptr;
            }
        }

        ImGui::Separator();
       
        // Style editor.
        static bool style = false;
        if (ImGui::Checkbox(" Style Editor", &style)) 
        {
            static std::string styleLayernName;
            if (style)
            {
                auto* layer = app.pushGuiLayer<ImGuiDebugWindow>("Style Editor", LumenDockPanel::Floating, 0, false);
                layer->getGui()->showStyleEditor = true;
                styleLayernName = layer->getName();
            }
            else
            {
                app.queuePopLayer(styleLayernName);
            }
        }

        // ImGui demo window.
        static bool demo = false;
        if (ImGui::Checkbox(" Demo Window", &demo))
        {
            static std::string demoLayerName;
            if (demo)
            {
                auto* layer = app.pushGuiLayer<ImGuiDebugWindow>("Demo Window", LumenDockPanel::Floating, 0, false);
                layer->getGui()->showDemoWindow = true;
                demoLayerName = layer->getName();
            }
            else
            {
                app.queuePopLayer(demoLayerName);
            }
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Add BGCE"))
        {
            app.pushGuiLayer<BackgroundColorEditor>("BGCE", LumenDockPanel::Floating);
        }

        // End.
        ImGui::EndMenu();

        ImGui::PopStyleVar();
    }
};

void Toolbar::end()
{
    // End.
    ImGui::EndMainMenuBar();
    // Style.
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
}

/*=======================================================================================================================================*/
/* EOF.                                                                                                                                  */
/*=======================================================================================================================================*/