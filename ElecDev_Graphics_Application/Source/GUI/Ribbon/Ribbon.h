#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <Windows.h>
#include <glad/glad.h>
#include <imgui/imgui.h>
#include <string>
#include "Application/LumenWindow/LumenWindow.h"
#include "Resources/ResourceHandler.h"

//==============================================================================================================================================//
//  Ribbon.																																		//
//==============================================================================================================================================//

class Ribbon : public LumenWindow
{

public:
	
	// Constructor.
	Ribbon(std::string name, int windowFlags = 0);

	// Rendering.
	virtual void onImGuiBegin() override;
	virtual void onImGuiRender() override;
	virtual void onImGuiEnd() override;

private:

	// -------- //
	//  D A T A //
	// -------- //

	unsigned m_loadFileIcon = NULL;
	unsigned m_circuitIcon = NULL;
	unsigned m_componentIcon = NULL;
	unsigned m_settingsIcon = NULL;
	unsigned m_userIcon = NULL;
	unsigned m_cableIcon = NULL;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//