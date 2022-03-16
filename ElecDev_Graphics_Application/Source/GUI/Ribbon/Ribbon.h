#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <Windows.h>
#include <glad/glad.h>
#include <imgui/imgui.h>
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
	virtual void onRender() override;
	virtual void end() override;

private:

	// -------- //
	//  D A T A //
	// -------- //

	unsigned m_loadFileIcon = NULL;
	unsigned m_circuitIcon = NULL;
	unsigned m_componentIcon = NULL;
	unsigned m_settingsIcon = NULL;
	unsigned m_userIcon = NULL;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//