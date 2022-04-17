#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Application/LumenWindow/LumenWindow.h"
#include <filesystem>
#include <string>

//==============================================================================================================================================//
//  Forward declerations.																														//
//==============================================================================================================================================//

class AssetViewer;

//==============================================================================================================================================//
//  Popup menu.																																	//
//==============================================================================================================================================//

class AssetExplorer : public LumenWindow
{
public:

	// Constructor.
	AssetExplorer(std::string name, int imguiWindowFlags);
	// Destructor.
	virtual ~AssetExplorer();
	
	// Rendering.
	virtual void onImGuiBegin() override;
	virtual void onImGuiRender() override;
	virtual void onImGuiEnd() override;

private:

	// The directory to open.
	static std::string s_startingDirectory;
	static std::filesystem::path m_currentDirectory;

	// Icons.
	inline static unsigned s_folderIcon = NULL;
	inline static unsigned s_fileIcon = NULL;
	inline static unsigned s_cableIcon = NULL;
	inline static unsigned s_circuitFileIcon = NULL;
	inline static unsigned s_leftArrowIcon = NULL;
	inline static unsigned s_upArrowIcon = NULL;
	inline static unsigned s_componentFileIcon = NULL;
	inline static unsigned s_reloadIcon = NULL;
	
	bool m_displayComponents = true;
	bool m_displayCircuits = true;
	bool m_displayCables = true;

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
	std::vector<std::filesystem::path> m_pathUndoHistory;

	// Engine used to view assets.
	AssetViewer* m_assetViewerEngine = nullptr;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//