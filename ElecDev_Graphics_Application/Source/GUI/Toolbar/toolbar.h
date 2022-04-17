#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Application/LumenWindow/LumenWindow.h"

//==============================================================================================================================================//
//  Forward declerations.																														//
//==============================================================================================================================================//

class AssetExplorer;

//==============================================================================================================================================//
//  Toolbar.																																	//
//==============================================================================================================================================//

class Toolbar : public LumenWindow
{

public:

	// Constructor.
    Toolbar(std::string& name, int windowFlags);

	// Rendering.
	virtual void onImGuiBegin() override;
	virtual void onImGuiRender() override;
	virtual void onImGuiEnd() override;

	//  Data for the lumen logo.
	float m_texWidth = 0;
	float m_texHeight = 0;
	unsigned m_texID = 0;

	glm::vec4 m_colour;

	// Asset explorer.
	AssetExplorer* m_assetExplorerWindow = nullptr;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//