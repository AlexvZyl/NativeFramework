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
	inline virtual ~AssetExplorer() = default;

	// Rendering.
	virtual void begin() override;
	virtual void onRender() override;
	virtual void end() override;

private:

	// The directory to open.
	static std::string s_directory;

	static unsigned s_folderIcon;
	static unsigned s_fileIcon;
	static unsigned s_circuitFileIcon;
	static unsigned s_leftArrowIcon;
	static unsigned s_componentFileIcon;
	glm::vec4 m_buttonColor;
	glm::vec4 m_borderColor;
	std::filesystem::path m_currentDirectory;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//