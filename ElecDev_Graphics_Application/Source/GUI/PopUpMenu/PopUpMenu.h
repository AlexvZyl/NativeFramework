#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Application/LumenWindow/LumenPopupWindow.h"
#include "glm/glm.hpp"

//==============================================================================================================================================//
//  Forward declerations.																														//
//==============================================================================================================================================//

class Base2DEngine;

//==============================================================================================================================================//
//  Popup menu.																																	//
//==============================================================================================================================================//

class PopUpMenu : public LumenPopupWindow
{
public:

	// Constructor.
	PopUpMenu(const std::string& name, int imguiWindowFlags = 0);

	// Rendering.
	virtual void onImGuiRender() override;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//