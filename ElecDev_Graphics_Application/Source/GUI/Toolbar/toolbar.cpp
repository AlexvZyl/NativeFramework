/*=======================================================================================================================================*/
/* Includes                                                                                                                              */
/*=======================================================================================================================================*/

#include <iostream>
#include <cmath>
#include <cfenv>
#include "toolbar.h"
#include "Utilities/Windows/WindowsUtilities.h"
#include "Lumen/Lumen.h"
#include "Application/Application.h"
#include "Application/ApplicationTemplates.h"
#include "Application/Events/EventLog.h"
#include "Resources/ResourceHandler.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "GUI/RendererStats/RendererStats.h"
#include "GUI/AssetExplorer/AssetExplorer.h"
#include "GUI/ImGuiDemoWindow/ImGuiDebugWindow.h"
#include "GUI/SceneHierarchy/SceneHierarchy.h"
#include "GUI/BackgroundColorEditor/BackgroundColorEditor.h"
#include <GLFW/glfw3.h>

/*=======================================================================================================================================*/
/* Constructor.                                                                                                                          */
/*=======================================================================================================================================*/

// Constructor.
Toolbar::Toolbar(const std::string& name, int windowFlags)
    : LumenWindow(name, windowFlags)
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

void Toolbar::onImGuiBegin()
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

void Toolbar::onImGuiRender()
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
                EventLog::log<FileLoadEvent>(fsPath.string(), EventType_Application);
            }
        }

        // Save file.
        if (ImGui::MenuItem("Save...", "Ctrl+S", nullptr, (bool)app.getActiveEngine()))
        {
            // Create and log save event.
            auto path = selectFile("Lumen Save Circuit", "", app.getActiveEngine()->m_parentWindow->getName(), "Save");
            if (path.string().size())
            {
                EventLog::log<FileSaveEvent>(path.string());
                app.getActiveEngine()->savedDocument();
            }
        }

        ImGui::Separator();

        // Close.
        if (ImGui::MenuItem("Close", "Ctrl+W")) Lumen::getApp().stopRunning();

        ImGui::PopStyleVar();
        ImGui::EndMenu();
    }

    // --------- //
    //  E D I T  //
    // --------- //

    ImGui::SetCursorPosY(m_contentRegionPosition.y);

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

        ImGui::PopStyleVar();
        ImGui::EndMenu();
    }

    // ----------- //
    //  T O O L S  //
    // ----------- //

    ImGui::SetCursorPosY(m_contentRegionPosition.y);

    if(ImGui::BeginMenu("Tools"))
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, dropdownItemSpacing);

        // Renderer info.
        static bool profilerState = false;
        if (ImGui::Checkbox(" Renderer Info", &profilerState))
        {
            static RendererStats* stats = nullptr;
            if (profilerState)
            {
                stats = app.pushWindow<RendererStats>(LumenDockPanel::Right, "Renderer Info");
                app.m_profilerActive = true;
            }
            else 
            {
                app.m_profilerActive = false;
                stats->closeWindow();
                stats = nullptr;
            }
        }   

        // Scene hierarchy.
        static bool sceneHierarchyBool = false;
        if (ImGui::Checkbox(" Scene Hierarchy", &sceneHierarchyBool))
        {
            static SceneHierarchy* sceneHierarchy = nullptr;
            if (sceneHierarchyBool)
            {
                sceneHierarchy = app.pushWindow<SceneHierarchy>(LumenDockPanel::Right, "Scene Hierarchy");
            }
            else
            {
                sceneHierarchy->closeWindow();
                sceneHierarchy = nullptr;
            }
        }

        // Asset explorer.
        static bool assetExplorer = true;
        if (ImGui::Checkbox(" Asset Explorer", &assetExplorer))
        {
            if (assetExplorer)
            {
                m_assetExplorerWindow = app.pushWindow<AssetExplorer>(LumenDockPanel::Bottom, "Asset Explorer");
                app.m_assetViewerEngine = m_assetExplorerWindow->m_assetViewerEngine;
            }
            else
            {
                m_assetExplorerWindow->closeWindow();
                m_assetExplorerWindow = nullptr;
                app.m_assetViewerEngine = nullptr;
            }
        }

        ImGui::Separator();
       
        // Style editor.
        static bool style = false;
        if (ImGui::Checkbox(" Style Editor", &style)) 
        {
            static ImGuiDebugWindow* styleWindow = nullptr;
            if (style)
            {
                styleWindow = app.pushWindow<ImGuiDebugWindow>(LumenDockPanel::Floating, "Style Editor");
                styleWindow->showStyleEditor = true;
            }
            else
            {
                app.queueWindowPop(styleWindow);
                styleWindow = nullptr;
            }
        }

        // ImGui demo window.
        static bool demo = false;
        if (ImGui::Checkbox(" Demo Window", &demo))
        {
            static ImGuiDebugWindow* demoWindow = nullptr;
            if (demo)
            {
                demoWindow = app.pushWindow<ImGuiDebugWindow>(LumenDockPanel::Floating, "Demo Window");
                demoWindow->showDemoWindow = true;
            }
            else
            {
                app.queueWindowPop(demoWindow);
                demoWindow = nullptr;
            }
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Add BGCE"))
        {
            app.pushWindow<BackgroundColorEditor>(LumenDockPanel::Floating, "BGCE");
        }

        // End.
        ImGui::PopStyleVar();
        ImGui::EndMenu();
    }
};

void Toolbar::onImGuiEnd()
{
    // End.
    ImGui::EndMainMenuBar();
    // Style.
    ImGui::PopStyleVar(4);
    ImGui::PopStyleColor();
}

/*=======================================================================================================================================*/
/* EOF.                                                                                                                                  */
/*=======================================================================================================================================*/