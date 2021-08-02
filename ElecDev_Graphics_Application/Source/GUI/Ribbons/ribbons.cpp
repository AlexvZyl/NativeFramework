

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


/*=======================================================================================================================================*/
/* Declarations                                                                                                                          */
/*=======================================================================================================================================*/


// Constructor.
Ribbons::Ribbons() {

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
    this->my_tool_active = true;

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
}


void Ribbons::renderRibbons()
{

    // Menu
    ImGui::Begin("Ribbons", &this->my_tool_active, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::SetWindowPos(ImVec2(0, 50));

    //Create Image Buttons
    if (ImGui::ImageButton((void*)image1_texture, ImVec2(50, 50))){

        ImGui::SetWindowPos(ImVec2(0, 50));
            
    }

    Ribbons::BeginButtonDropDown("Draw", "Draw\nMCC", ImVec2(50,50));

    if (ImGui::ImageButton((void*)image2_texture, ImVec2(50, 50))) {

        ImGui::SetWindowPos(ImVec2(0, 50));

    }
    Ribbons::BeginButtonDropDown("Block", "Bloack\nDiagram", ImVec2(50, 50));

    if (ImGui::ImageButton((void*)image3_texture, ImVec2(50, 50))) {

        ImGui::SetWindowPos(ImVec2(0, 50));

    }
    Ribbons::BeginButtonDropDown("Bucket", "Draw\nCircuit\nBuckets", ImVec2(50, 50));




    ImGui::End();


}

bool Ribbons::BeginButtonDropDown(const char* label, const char* name, ImVec2 buttonSize)
{

    ImGuiWindow* window = ImGui::GetCurrentWindow();
    auto& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    float x = ImGui::GetCursorPosX();
    float y = ImGui::GetCursorPosY();

    ImVec2 size(buttonSize.x, buttonSize.x/2);
    bool pressed = ImGui::Button(name);

    // Arrow
    ImVec2 center(window->Pos.x + x + buttonSize.x, window->Pos.y + y + buttonSize.y / 4);
    float r = 8.f;
    center.y += r * 0.25f;
    ImVec2 a = center + ImVec2(0, 1) * r;
    ImVec2 b = center + ImVec2(-0.866f, -0.5f) * r;
    ImVec2 c = center + ImVec2(0.866f, -0.5f) * r;

    window->DrawList->PathClear();
    window->DrawList->AddTriangleFilled(a, b, c, ImGui::GetColorU32(ImGuiCol_Text));

    // Popup

    if (pressed)
    {
        ImGui::OpenPopup(label);
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




