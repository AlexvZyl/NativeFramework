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
    // Remove rounding so that it docks nicely.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    // Set the size of the toolbar.
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, TOOLBAR_PADDING));
    // Begin.
    m_isOpen = ImGui::BeginMainMenuBar();
}

void Toolbar::onRender()
{
    // Get the app.
    Application& app = Lumen::getApp();
    
    // Draw the image.
    float textureSize = ImGui::GetFont()->FontSize + 2 * TOOLBAR_PADDING - 3;
    ImGui::Image((void*)m_texID, ImVec2(textureSize, textureSize), ImVec2(0, 1), ImVec2(1, 0));

    // --------- //
    //  F I L E  //
    // --------- //

    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Load..", "Ctrl+O"))
        {
            // Create a load event.
            std::string path = selectFile("Lumen Load Circuit", "", "", "Load");
            FileLoadEvent event(path);
            app.logEvent<FileLoadEvent>(event);
        }
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
        if (ImGui::MenuItem("Undo", "CTRL+Z"))
        {

        }
        if (ImGui::MenuItem("Redo", "CTRL+Y", false, false))
        {

        }

        ImGui::Separator();

        if (ImGui::MenuItem("Cut", "CTRL+X"))
        {

        }
        if (ImGui::MenuItem("Copy", "CTRL+C"))
        {

        }
        if (ImGui::MenuItem("Paste", "CTRL+V"))
        {

        }

        ImGui::EndMenu();
    }
};

void Toolbar::end()
{
    ImGui::EndMainMenuBar();
    // Pop frame rounding and padding.
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
}

/*=======================================================================================================================================*/
/* EOF.                                                                                                                                  */
/*=======================================================================================================================================*/