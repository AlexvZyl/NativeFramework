#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "GUI/GuiElementCore/GuiElementCore.h"
#include <filesystem>

//==============================================================================================================================================//
//  Popup menu.																																	//
//==============================================================================================================================================//

class AssetExplorer : public GuiElementCore
{
public:


	// Constructor.
	AssetExplorer(std::string name, int imguiWindowFlags);
	// Destructor.
	inline virtual ~AssetExplorer() 
	{
		s_startingDirectory = m_currentDirectory.string();
	};

	// Rendering.
	virtual void begin() override;
	virtual void onRender() override;
	virtual void end() override;

private:

	// The directory to open.
	static std::string s_startingDirectory;
	static std::filesystem::path m_currentDirectory;

	static unsigned s_folderIcon;
	static unsigned s_fileIcon;
	static unsigned s_circuitFileIcon;
	static unsigned s_leftArrowIcon;
	static unsigned s_componentFileIcon;
	static unsigned s_reloadIcon;
	glm::vec4 m_buttonColor;
	glm::vec4 m_borderColor;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//