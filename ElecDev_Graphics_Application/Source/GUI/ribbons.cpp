/*=======================================================================================================================================*/
/* Includes                                                                                                                             */
/*=======================================================================================================================================*/

#include <Core/imgui.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <cfenv>
#include "ribbons.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <Core/imgui_internal.h>

/*=======================================================================================================================================*/
/* Functions                                                                                                                             */
/*=======================================================================================================================================*/





/*=======================================================================================================================================*/
/* Declarations                                                                                                                          */
/*=======================================================================================================================================*/
void EndButtonDropDown();
bool BeginButtonDropDown(const char* label, ImVec2 buttonSize);
bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);


/*=======================================================================================================================================*/
/* Flags                                                                                                                             */
/*=======================================================================================================================================*/

static bool show_app_main_menu_bar = false;
static bool show_app_documents = false;

static bool show_app_console = false;
static bool show_app_log = false;
static bool show_app_layout = false;
static bool show_app_property_editor = false;
static bool show_app_long_text = false;
static bool show_app_auto_resize = false;
static bool show_app_constrained_resize = false;
static bool show_app_simple_overlay = false;
static bool show_app_fullscreen = false;
static bool show_app_window_titles = false;
static bool show_app_custom_rendering = false;

static bool show_app_metrics = false;
static bool show_app_style_editor = false;
static bool show_app_about = false;

static bool no_titlebar = false;
static bool no_scrollbar = false;
static bool no_menu = false;
static bool no_move = false;
static bool no_resize = false;
static bool no_collapse = false;
static bool no_close = false;
static bool no_nav = false;
static bool no_background = false;
static bool no_bring_to_front = false;
static bool unsaved_document = false;
static bool my_tool_active = true;


// Simple helper function to load an image into a OpenGL texture with common settings
bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
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





void renderRibbons()
{

    // Menu
    ImGui::Begin("Ribbons", &my_tool_active, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::SetWindowPos(ImVec2(0, 50));

    //Load textures
    int image1_width = 0;
    int image1_height = 0;
    GLuint image1_texture = 0;
    bool ret1 = LoadTextureFromFile("Source\\GUI\\Images\\Draw_MCC.png", &image1_texture, &image1_width, &image1_height);
    IM_ASSERT(ret1);

    int image2_width = 0;
    int image2_height = 0;
    GLuint image2_texture = 0;
    bool ret2 = LoadTextureFromFile("Source\\GUI\\Images\\component.png", &image2_texture, &image2_width, &image2_height);
    IM_ASSERT(ret2);

    int image3_width = 0;
    int image3_height = 0;
    GLuint image3_texture = 0;
    bool ret3 = LoadTextureFromFile("Source\\GUI\\Images\\Draw_Circuit_buckets.png", &image3_texture, &image3_width, &image3_height);
    IM_ASSERT(ret3);

    //Create Image Buttons

    ImGui::Begin("Draw MCC", &my_tool_active, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize); ImGui::SameLine();
        /*ImGui::SetWindowPos(ImVec2(0, 50));
        ImGui::ImageButton((void*)(intptr_t)image1_texture, ImVec2(50, 50)); 
        ImGui::Button("Draw MCC", ImVec2(50, 50));  
        ImGui::Button("Redraw MCC", ImVec2(50, 50));
        ImGui::Button("Draw in AutoCAD", ImVec2(50, 50));*/
    ImGui::End();

    /*ImGui::Begin("Block Diagram", &my_tool_active, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize); ImGui::SameLine();
        ImGui::ImageButton((void*)(intptr_t)image2_texture, ImVec2(50, 50));
        ImGui::Button("Draw MCC", ImVec2(50, 50));
    ImGui::End();

    ImGui::Begin("Draw Circuit Buckets", &my_tool_active, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize); ImGui::SameLine();
        ImGui::ImageButton((void*)(intptr_t)image3_texture, ImVec2(50, 50));
        ImGui::Button("Draw MCC", ImVec2(50, 50));
    ImGui::End();
    */

    ImGui::End();


}

//bool BeginButtonDropDown(const char* label, ImVec2 buttonSize)
//{
//    ImGui::SameLine(0.f, 0.f);
//
//    ImGuiWindow* window = ImGui::GetCurrentWindow();
//    ImGuiState& g = *GImGui;
//    const ImGuiStyle& style = g.Style;
//
//    float x = ImGui::GetCursorPosX();
//    float y = ImGui::GetCursorPosY();
//
//    ImVec2 size(20, buttonSize.y);
//    bool pressed = ImGui::Button("##", size);
//
//    // Arrow
//    ImVec2 center(window->Pos.x + x + 10, window->Pos.y + y + buttonSize.y / 2);
//    float r = 8.f;
//    center.y -= r * 0.25f;
//    ImVec2 a = center + ImVec2(0, 1) * r;
//    ImVec2 b = center + ImVec2(-0.866f, -0.5f) * r;
//    ImVec2 c = center + ImVec2(0.866f, -0.5f) * r;
//
//    window->DrawList->AddTriangleFilled(a, b, c, GetColorU32(ImGuiCol_Text));
//
//    // Popup
//
//    ImVec2 popupPos;
//
//    popupPos.x = window->Pos.x + x - buttonSize.x;
//    popupPos.y = window->Pos.y + y + buttonSize.y;
//
//    ImGui::SetNextWindowPos(popupPos);
//
//    if (pressed)
//    {
//        ImGui::OpenPopup(label);
//    }
//
//    if (ImGui::BeginPopup(label))
//    {
//        ImGui::PushStyleColor(ImGuiCol_FrameBg, style.Colors[ImGuiCol_Button]);
//        ImGui::PushStyleColor(ImGuiCol_WindowBg, style.Colors[ImGuiCol_Button]);
//        ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, style.Colors[ImGuiCol_Button]);
//        return true;
//    }
//
//    return false;
//}
//
//void EndButtonDropDown()
//{
//    ImGui::PopStyleColor(3);
//    ImGui::EndPopup();
//}




