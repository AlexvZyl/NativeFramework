#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <Windows.h>
#include <glad/glad.h>
#include <Core/imgui.h>
#include <string>
#include "GUI/GuiElementCore/GuiElementCore.h"
#include "Resources/ResourceHandler.h"

//==============================================================================================================================================//
//  Ribbon.																																		//
//==============================================================================================================================================//

class Ribbon : public GuiElementCore
{

public:
	
	// Constructor.
	Ribbon(std::string name, int windowFlags);

	// Rendering.
	virtual void begin() override;
	virtual void renderBody() override;
	virtual void end() override;

private:

	// -------- //
	//  D A T A //
	// -------- //

	bool my_tool_active = true;
	bool drawToggle = true;
    int image1_width;
    int image1_height;
    GLuint image1_texture;
    bool ret1;
    int image2_width;
    int image2_height;
    GLuint image2_texture;
    bool ret2;
    int image3_width;
    int image3_height;
    GLuint image3_texture;
	int image4_width;
	int image4_height;
	GLuint image4_texture;
    bool ret3;
	std::string sideBarFlag;
	ImVec2 MCCPos;
	ImGuiID* dock;
	ImGuiID left;
	bool first[3];
	
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//