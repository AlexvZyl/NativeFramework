/*=======================================================================================================================================*/
/* Includes                                                                                                                              */
/*=======================================================================================================================================*/

#include "ribbons.h"
#include <Core/imgui.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <cfenv>
#define STB_IMAGE_IMPLEMENTATION
#include <External/Misc/stb_image.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <Core/imgui_internal.h>

/*=======================================================================================================================================*/
/* Declarations                                                                                                                          */
/*=======================================================================================================================================*/

// Constructor.
Ribbons::Ribbons()
{
    this->show_app_main_menu_bar = false;
    this->show_app_documents = false;
    this->show_app_console = false;
    this->show_app_log = false;
    this->show_app_layout = false;
    this->show_app_property_editor = false;
    this->show_app_long_text = false;
    this->show_app_auto_resize = false;
    this->show_app_constrained_resize = false;
    this->show_app_simple_overlay = false;
    this->show_app_fullscreen = false;
    this->show_app_window_titles = false;
    this->show_app_custom_rendering = false;
    this->show_app_metrics = false;
    this->show_app_style_editor = false;
    this->show_app_about = false;
    this->no_titlebar = false;
    this->no_scrollbar = false;
    this->no_menu = false;
    this->no_move = false;
    this->no_resize = false;
    this->no_collapse = false;
    this->no_close = false;
    this->no_nav = false;
    this->no_background = false;
    this->no_bring_to_front = false;
    this->unsaved_document = false;
    this->my_tool_active = false;
    this->drawToggle = false;

    // Load texture 1. 
    BITMAP texture1BM = loadImageFromResource(DRAW_MCC_PNG);
    this->image1_width = texture1BM.bmWidth;
    this->image1_height = texture1BM.bmHeight;
    this->image1_texture = loadBitmapToGL(texture1BM);

    // Load texture 2.
    BITMAP texture2BM = loadImageFromResource(COMPONENT_PNG);
    this->image2_width = texture2BM.bmWidth;
    this->image2_height = texture2BM.bmHeight;
    this->image2_texture = loadBitmapToGL(texture2BM);

    // Load texture 3.
    BITMAP texture3BM = loadImageFromResource(DRAW_CIRCUIT_BUCKETS_PNG);
    this->image3_width = texture3BM.bmWidth;
    this->image3_height = texture3BM.bmHeight;
    this->image3_texture = loadBitmapToGL(texture3BM);

    // Load texture 4. 
    BITMAP texture4BM = loadImageFromResource(CIRCUIT_CIRCLE_JPEG);
    this->image4_width = texture4BM.bmWidth;
    this->image4_height = texture4BM.bmHeight;
    this->image4_texture = loadBitmapToGL(texture4BM);

    this->sideBarFlag = "";
    first[0] = true;
    first[1] = true;
    first[2] = true;
}

void Ribbons::renderRibbons(ImGuiID* dock)
{
    this->dock = dock;
    topBar();
    sideBar();
}

bool Ribbons::topBar()
{
    // Menu
    bool* p_open = NULL;
    ImGui::SetNextWindowBgAlpha(1);
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoFocusOnAppearing;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
    ImVec2 work_size = viewport->WorkSize;
    m_guiState->toolsExpanded = ImGui::Begin("Tools", p_open, window_flags);

    ImGui::SetWindowSize(ImVec2(work_size.x, 70));
    ImGui::SetWindowPos(work_pos);
    // Create Image Buttons.
    if (ImGui::ImageButton((void*)image1_texture, ImVec2(30, 30)))
    {
        if (this->sideBarFlag == "Draw MCC") { this->sideBarFlag = "None"; }
        else { this->sideBarFlag = "Draw MCC"; }
    }
    ImGui::SameLine();
    if (ImGui::ImageButton((void*)image2_texture, ImVec2(30, 30)))
    {
        if (this->sideBarFlag == "Block Diagram") { this->sideBarFlag = "None"; }
        else { this->sideBarFlag = "Block Diagram"; }
        m_guiState->blockDiagram = true;
    }
    ImGui::SameLine();
    if (ImGui::ImageButton((void*)image3_texture, ImVec2(30, 30))) { this->sideBarFlag = "Draw Circuit Bucket"; }
    ImGui::SameLine();
    // Add Design engine button.
    if (ImGui::ImageButton((void*)image4_texture, ImVec2(30, 30))) { m_guiState->circuitEditor = true; }
    ImGui::End();
    return true;
}

bool Ribbons::sideBar()
{
    if (this->sideBarFlag == "Block Diagram") { blockDiagram(); }
    if (this->sideBarFlag == "Draw MCC") { MCC(); }
    return true;
}

bool Ribbons::MCC()
{
    bool close = true;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
    ImVec2 work_size = viewport->WorkSize;
    if (first[1])
    {
        ImGui::SetNextWindowDockID(this->left, ImGuiCond_Once);
        first[1] = false;
    }
    ImGui::Begin("Drawing MCC", &close, ImGuiWindowFlags_NoResize);
    if (!close) { this->sideBarFlag = "None"; }
    ImGui::Button("New MCC");
    ImGui::Separator();
    ImGui::Button("Redraw MCC");
    ImGui::Separator();
    ImGui::Button("Draw in AutoCAD");
    ImGui::End();
    return true;
}

bool Ribbons::blockDiagram()
{
    bool close = true;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
    ImVec2 work_size = viewport->WorkSize;
    if (first[2])
    {
        ImGui::SetNextWindowDockID(this->left, ImGuiCond_Once);
        first[2] = false;
    }
    ImGui::Begin("Drawing GUI", &close);
    if (!close)
    {
        this->sideBarFlag = "None";
    }
    ImGui::Text("General:");
    ImGui::Button("Recenter"); ImGui::SameLine();
    ImGui::Button("Generate Dataframe");
    ImGui::Separator();
    ImGui::Text("Circuit:");
    ImGui::Button("Remove Component"); ImGui::SameLine();
    ImGui::Button("Done"); ImGui::SameLine();
    ImGui::Button("Remove All");
    ImGui::Separator();
    ImGui::Text("Component Removal:");
    ImGui::Button("New Cable"); ImGui::SameLine();
    ImGui::Button("Done"); ImGui::SameLine();
    ImGui::Button("Undo");
    ImGui::Separator();
    ImGui::Text("Component Generation:");
    ImGui::Text("Component Name:"); ImGui::SameLine();
    static char str0[128] = "";
    ImGui::InputText("", str0, IM_ARRAYSIZE(str0));
    ImGui::Text("Ground"); ImGui::SameLine();
    const char* items[] = { "None", "Bottom Left", "Bottom Right" };
    static const char* current_item = NULL;
    if (ImGui::BeginCombo("##combo", current_item)) // The second parameter is the label previewed before opening the combo.
    {
        for (int n = 0; n < IM_ARRAYSIZE(items); n++)
        {
            bool is_selected = (current_item == items[n]); // You can store your selection however you want, outside or inside your objects
            if (ImGui::Selectable(items[n], is_selected))
                current_item = items[n];
            if (is_selected)
                ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
        }
        ImGui::EndCombo();
    }
    ImGui::Button("New Cable"); ImGui::SameLine();
    ImGui::Button("Done"); ImGui::SameLine();
    ImGui::Button("Undo");
    ImGui::End();
    return true;
}

bool Ribbons::BeginDrawButtonDropDown(const char* label, ImVec2 buttonSize)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    auto& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    float x = ImGui::GetCursorPosX();
    float y = ImGui::GetCursorPosY();
    ImVec2 size(buttonSize.x, buttonSize.x / 2);
    bool pressed = ImGui::Button("Draw\nMCC", buttonSize);
    // Arrow
    ImVec2 center(window->Pos.x + x + buttonSize.x * 0.9, window->Pos.y + y + buttonSize.y / 4);
    float r = 8.f;
    center.y += r * 0.25f;
    ImVec2 a = center + ImVec2(0, 1) * r;
    ImVec2 b = center + ImVec2(-0.866f, -0.5f) * r;
    ImVec2 c = center + ImVec2(0.866f, -0.5f) * r;
    window->DrawList->PathClear();
    window->DrawList->AddTriangleFilled(a, b, c, ImGui::GetColorU32(ImGuiCol_Text));
    // Popup
    ImVec2 popupPos;
    popupPos.x = window->Pos.x + x + buttonSize.x / 4;
    popupPos.y = window->Pos.y + y + buttonSize.y;
    ImGui::SetNextWindowPos(popupPos);
    if (pressed) { this->drawToggle = !this->drawToggle; }
    if (this->drawToggle)
    {
        ImGui::Begin("Draw MCC", &this->my_tool_active, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
        //ImGui::SetWindowPos(popupPos);
        ImGui::Button("asjdjd");
        ImGui::End();
    }
    return false;
}

/*=======================================================================================================================================*/
/* EOF.                                                                                                                                  */
/*=======================================================================================================================================*/