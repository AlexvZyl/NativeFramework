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

	// Icons.
	static unsigned s_folderIcon;
	static unsigned s_fileIcon;
	static unsigned s_circuitFileIcon;
	static unsigned s_leftArrowIcon;
	static unsigned s_componentFileIcon;
	static unsigned s_reloadIcon;
	
	bool m_displayComponents = true;
	bool m_displayCircuits = true;

	// Reload the files in the current directory.
	void loadDirectories();
	// The directories to be displayed.
	std::vector<std::filesystem::directory_entry> m_directories;

	// Used to make filter clearing more elegant.
	bool m_clearFilterOnFrameStart = false;
	// Reload the directories at the start of rendering.
	bool m_reloadDirectories = false;

	// Keep track of the file history for the forward button.
	std::vector<std::filesystem::path> m_pathHistory;

};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//