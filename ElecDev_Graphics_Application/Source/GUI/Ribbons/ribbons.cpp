

#include "ribbons.h"
#include <Core/imgui.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <cfenv>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <Core/imgui_internal.h>
#include "../Helper/stateMachine.h"


/*=======================================================================================================================================*/
/* Declarations                                                                                                                          */
/*=======================================================================================================================================*/


// Constructor.
Ribbons::Ribbons(stateMachine states) {

    this->states = states;

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

    //Load textures
    this->image1_width = 0;
    this->image1_height = 0;
    this->image1_texture = 0;
    this->ret1 = this->LoadTextureFromFile("Source\\GUI\\Images\\Draw_MCC.png", &this->image1_texture, &this->image1_width, &this->image1_height);
    IM_ASSERT(ret1);

    this->image2_width = 0;
    this->image2_height = 0;
    this->image2_texture = 0;
    this->ret2 = this->LoadTextureFromFile("Source\\GUI\\Images\\component.png", &this->image2_texture, &this->image2_width, &this->image2_height);
    IM_ASSERT(ret2);

    this->image3_width = 0;
    this->image3_height = 0;
    this->image3_texture = 0;
    this->ret3 = this->LoadTextureFromFile("Source\\GUI\\Images\\Draw_Circuit_buckets.png", &this->image3_texture, &this->image3_width, &this->image3_height);
    IM_ASSERT(ret3);

    this->sideBarFlag = "";
}


void Ribbons::renderRibbons()
{
    
    topBar();
    sideBar();

}

bool Ribbons::topBar() {

    // Menu
    ImVec2 mainWindow = ImGui::GetMainViewport()->WorkSize;
    bool* p_open = NULL;
    this->states.toolsExpanded = ImGui::Begin("Tools", p_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
    
    ImGui::SetWindowSize(ImVec2(mainWindow.x+1,70));
    ImGui::SetWindowPos(ImVec2(-1, 18));

    //Create Image Buttons
    if (ImGui::ImageButton((void*)image1_texture, ImVec2(30, 30))) {\

        if (this->sideBarFlag == "Draw MCC") {

            this->sideBarFlag = "None";
        }
        else {

            this->sideBarFlag = "Draw MCC";
        }
    }
    ImGui::SameLine();

    if (ImGui::ImageButton((void*)image2_texture, ImVec2(30, 30))) {

        if (this->sideBarFlag == "Block Diagram") {
            this->sideBarFlag = "None";
        }
        else {
            this->sideBarFlag = "Block Diagram";
        }

        

    }
    ImGui::SameLine();

    if (ImGui::ImageButton((void*)image3_texture, ImVec2(30, 30))) {

        this->sideBarFlag = "Draw Circuit Bucket";

    }
    ImGui::SameLine();

    ImGui::End();

    return true;

}

bool Ribbons::sideBar() {

    if (this->sideBarFlag == "Block Diagram")
    {
        blockDiagram();

    }

    if (this->sideBarFlag == "Draw MCC")
    {
        MCC();
    }

    return true;

}

bool Ribbons::MCC() {
    bool close = true;
    ImGui::Begin("Drawing MCC", &close, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize);
    if (!close)
    {
        this->sideBarFlag = "None";
    }
    if (this->states.toolsExpanded) {
        ImGui::SetWindowPos(ImVec2(0, 90));
    }
    else {
        ImGui::SetWindowPos(ImVec2(0, 40));
    }

    ImGui::Button("New MCC");

    ImGui::Text("---------------------");

    ImGui::Button("Redraw MCC"); 

    ImGui::Text("---------------------");

    ImGui::Button("Draw in AutoCAD"); 

    ImGui::Text("---------------------");

    ImGui::End();

    return true;
}

bool Ribbons::blockDiagram() {

    bool close = true;
    ImGui::Begin("Drawing GUI", &close, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize);
    if (!close)
    {
        this->sideBarFlag = "None";
    }
    if (this->states.toolsExpanded) {
        ImGui::SetWindowPos(ImVec2(0, 90));
    }
    else {
        ImGui::SetWindowPos(ImVec2(0, 40));
    }
    

    ImGui::Text("General:");
    ImGui::Button("Recenter"); ImGui::SameLine();
    ImGui::Button("Generate Dataframe");

    ImGui::Text("------------------------------------");

    ImGui::Text("Circuit:");
    ImGui::Button("Remove Component"); ImGui::SameLine();
    ImGui::Button("Done"); ImGui::SameLine();
    ImGui::Button("Remove All");

    ImGui::Text("------------------------------------");

    ImGui::Text("Component Removal:");
    ImGui::Button("New Cable"); ImGui::SameLine();
    ImGui::Button("Done"); ImGui::SameLine();
    ImGui::Button("Undo");

    ImGui::Text("------------------------------------");

    ImGui::Text("Component Generation:");
    ImGui::Text("Component Name:"); ImGui::SameLine();
    static char str0[128] = "";
    ImGui::InputText("", str0, IM_ARRAYSIZE(str0));
    ImGui::Text("Ground"); ImGui::SameLine();
    const char* items[] = { "None", "Bottom Left", "Bottom Right"};
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

    ImGui::Text("------------------------------------");

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

    ImVec2 size(buttonSize.x, buttonSize.x/2);
    bool pressed = ImGui::Button("Draw\nMCC", buttonSize);

    // Arrow
    ImVec2 center(window->Pos.x + x + buttonSize.x*0.9, window->Pos.y + y + buttonSize.y / 4);
    float r = 8.f;
    center.y += r * 0.25f;
    ImVec2 a = center + ImVec2(0, 1) * r;
    ImVec2 b = center + ImVec2(-0.866f, -0.5f) * r;
    ImVec2 c = center + ImVec2(0.866f, -0.5f) * r;

    window->DrawList->PathClear();
    window->DrawList->AddTriangleFilled(a, b, c, ImGui::GetColorU32(ImGuiCol_Text));

    // Popup
    ImVec2 popupPos;

    popupPos.x = window->Pos.x + x + buttonSize.x/4;
    popupPos.y = window->Pos.y + y + buttonSize.y;

    ImGui::SetNextWindowPos(popupPos);

    if (pressed)
    {
        this->drawToggle = !this->drawToggle;
    }

    if (this->drawToggle)
    {
        
        ImGui::Begin("Draw MCC", &this->my_tool_active, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar| ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
        //ImGui::SetWindowPos(popupPos);
        ImGui::Button("asjdjd");
        ImGui::End();
    }


    return false;
}

// Simple helper function to load an image into a OpenGL texture with common settings
bool Ribbons::LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}




