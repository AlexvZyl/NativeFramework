//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Application/Application.h"
#include "AssetExplorer.h"
#include "Lumen.h"
#include "Application/Events/EventLog.h"
#include "Utilities/Windows/WindowsUtilities.h"
#include "Resources/ResourceHandler.h"
#include "imgui/notify/notify_icons_define.h"
#include "imgui/imgui_internal.h"

//==============================================================================================================================================//
//  Statics.																																	//
//==============================================================================================================================================//

std::string AssetExplorer::s_startingDirectory = getExecutableLocation(true);
std::filesystem::path AssetExplorer::m_currentDirectory;

//==============================================================================================================================================//
//  Popup menu.																																	//
//==============================================================================================================================================//

AssetExplorer::AssetExplorer(std::string name, int imguiWindowFlags)
	: GuiElementCore(name, imguiWindowFlags)
{
	m_currentDirectory = s_startingDirectory;
	// Load resources.
	s_fileIcon = loadBitmapToGL(loadImageFromResource(FILE_ICON));
	s_folderIcon = loadBitmapToGL(loadImageFromResource(FOLDER_ICON));
	s_circuitFileIcon = loadBitmapToGL(loadImageFromResource(CIRCUIT_FILE_ICON));
	s_leftArrowIcon = loadBitmapToGL(loadImageFromResource(LEFT_ARROW_ICON));
	s_componentFileIcon = loadBitmapToGL(loadImageFromResource(COMPONENT_FILE_ICON));
	s_reloadIcon = loadBitmapToGL(loadImageFromResource(RELOAD_ICON));
	s_upArrowIcon = loadBitmapToGL(loadImageFromResource(UP_ARROW_ICON));
	loadDirectories();
}

void AssetExplorer::begin()
{
	ImGui::Begin(m_name.c_str(), NULL, m_imguiWindowFlags);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {2.f, 10.f});
	ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 10.f);
	ImGui::PushStyleColor(ImGuiCol_Button, {0.f, 0.f, 0.f, 0.f});
}

void AssetExplorer::onRender()
{
	// Filter.
	static ImGuiTextFilter filter;
	if (m_clearFilterOnFrameStart)
	{
		m_clearFilterOnFrameStart = false;
		filter.Clear();
	}
	if (m_reloadDirectories)
	{
		loadDirectories();
		m_reloadDirectories = false;
	}

	static float buttonsWidth = 155.f;
	static float headerHeight = 41.f;
	static glm::vec2 headerSize(22, 22);

	// Buttons.
	if (ImGui::BeginChild("AssetButtons", { buttonsWidth, headerHeight }, true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
	{
		// Move back button.
		ImGui::PushID("BACK_AB");
		bool pathHistory = m_pathHistory.size();
		if(!pathHistory)
			ImGui::BeginDisabled();
		if (ImGui::ImageButton((void*)s_leftArrowIcon, headerSize, { 0, 0 }, { 1, 1 }))
		{
			if (pathHistory)
			{
				m_pathUndoHistory.push_back(m_currentDirectory);
				m_currentDirectory = m_pathHistory.back();
				m_pathHistory.pop_back();
				loadDirectories();
			}
		}
		if (!pathHistory)
			ImGui::EndDisabled();
		ImGui::PopID();

		// Move forward button.
		ImGui::SameLine();
		bool pathUndoHistory = m_pathUndoHistory.size();
		ImGui::PushID("FORWARD_AB");
		if (!pathUndoHistory)
			ImGui::BeginDisabled();
		if (ImGui::ImageButton((void*)s_leftArrowIcon, headerSize, { 1, 0 }, { 0, 1 }))
		{
			if (pathUndoHistory)
			{
				m_pathHistory.push_back(m_currentDirectory);
				m_currentDirectory = m_pathUndoHistory.back();
				m_pathUndoHistory.pop_back();
				loadDirectories();
			}
		}
		if (!pathUndoHistory)
			ImGui::EndDisabled();
		ImGui::PopID();

		// Move up.
		ImGui::PushID("UP_AB");
		ImGui::SameLine();
		if (ImGui::ImageButton((void*)s_upArrowIcon, headerSize, {0,1}, {1,0}))
		{
			if (m_currentDirectory != m_currentDirectory.parent_path())
			{
				m_pathHistory.push_back(m_currentDirectory);
				m_currentDirectory = m_currentDirectory.parent_path();
				loadDirectories();
			}
		}
		ImGui::PopID();

		// Reload button.
		ImGui::SameLine();
		if (ImGui::ImageButton((void*)s_reloadIcon, headerSize))
		{
			loadDirectories();
		}
	}
	ImGui::EndChild();

	ImGui::SameLine();

	// Current directory.
	if (ImGui::BeginChild("Dirctory", { (m_contentRegionSize.x - buttonsWidth) / 2.f, headerHeight }, true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
	{
		ImGui::SetScrollX(ImGui::GetScrollMaxX());
		if (ImGui::Button(m_currentDirectory.string().c_str(), { 0.f, headerSize.y + 7.f}))
		{
			auto newDirectory = selectFolder(m_currentDirectory.string());
			if (newDirectory.string().size())
			{
				m_pathHistory.push_back(m_currentDirectory);
				m_currentDirectory = newDirectory;
				m_clearFilterOnFrameStart = true;
				loadDirectories();
			}
		}
	}
	ImGui::EndChild();

	ImGui::SameLine();

	// Actions bar.
	static bool addFolder = false;
	if (ImGui::BeginChild("Actions", {50.f, headerHeight}, true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
	{
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.f);
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 3.f);
		ImGui::SetWindowFontScale(1.2f);
		if (ImGui::Button(ICON_FA_FOLDER_PLUS))
		{
			addFolder = true;
		}
		ImGui::SetWindowFontScale(1.f);
	}
	ImGui::EndChild();

	ImGui::SameLine();

	// Filter options.
	if (ImGui::BeginChild("AssetFilter", { 140.f, headerHeight }, true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
	{
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.f);
		if (ImGui::Checkbox(" .lmcp ", &m_displayComponents))
			loadDirectories();
		ImGui::SameLine();
		if (ImGui::Checkbox(" .lmct ", &m_displayCircuits))
			loadDirectories();
	}
	ImGui::EndChild();

	ImGui::SameLine();

	// Search bar.
	if (ImGui::BeginChild("SearchBar", { 0.f, headerHeight }, true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
	{
		static float textSize = ImGui::CalcTextSize("Search: ").x;
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6.f);
		ImGui::Text("Search: ");
		ImGui::SameLine();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3.f);
		filter.Draw("##AssetExplorerSearch", ImGui::GetWindowContentRegionWidth() - textSize);
	}
	ImGui::EndChild();

	// Files & Folders.
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 5.f, 5.f });
	if (ImGui::BeginChild("##AssetExplorerChild", {0,0}, true))
	{
		// Create icon columns.
		float iconSize = 75;
		float padding = 7;
		float cellSize = iconSize + 2 * padding;
		int columns = std::floor(m_contentRegionSize.x * 0.9 / cellSize);
		if (columns <= 0) columns = 1;
		ImGui::Columns(columns, 0, false);

		// Display contents.
		int directoryCount = 0;
		for (auto& p : m_directories)
		{
			// Filter.
			if (!filter.PassFilter(p.path().stem().string().c_str())) continue;

			ImGui::PushID(directoryCount++);

			// --------------- //
			//  F O L D E R S  //
			// --------------- //

			if (p.is_directory())
			{
				ImGui::ImageButton((void*)s_folderIcon, { iconSize, iconSize }, { 0, 1 }, { 1, 0 });
				if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
				{
					m_pathHistory.push_back(m_currentDirectory);
					m_currentDirectory /= p.path().filename();
					m_clearFilterOnFrameStart = true;
					m_reloadDirectories = true;
				}
			}
			// Files.
			else
			{
				// ----------------- //
				//  C I R C U I T S  //
				// ----------------- //

				if (p.path().extension() == ".lmct")
				{
					ImGui::ImageButton((void*)s_circuitFileIcon, { iconSize, iconSize }, { 0, 1 }, { 1, 0 });
					if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
					{
						EventLog::log<FileLoadEvent>(FileLoadEvent(p.path().string()));
					}
				}

				// --------------------- //
				//  C O M P O N E N T S  //
				// --------------------- //

				else if (p.path().extension() == ".lmcp")
				{
					ImGui::ImageButton((void*)s_componentFileIcon, { iconSize, iconSize }, { 0, 1 }, { 1, 0 });
					if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
					{
						// Open component builder.
					}
					// File drag & drop.
					if (ImGui::BeginDragDropSource())
					{
						const wchar_t* itemPath = p.path().c_str();
						ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t), ImGuiCond_Once);
						ImGui::EndDragDropSource();
					}
				}

				// ----------- //
				//  O T H E R  //
				// ----------- //

				else
				{
					ImGui::ImageButton((void*)s_fileIcon, { iconSize, iconSize }, { 0, 1 }, { 1, 0 });
				}
			}

			// The button label applied to all icons.
			std::string filename = p.path().filename().string().c_str();
			glm::vec2 stringSize = ImGui::CalcTextSize(filename.c_str());
			if (stringSize.x < cellSize)
				ImGui::SetCursorPosX(ImGui::GetCursorPos().x + (cellSize / 2) - (stringSize.x / 2));
			ImGui::TextWrapped(filename.c_str());

			// End.
			ImGui::NextColumn();
			ImGui::PopID();
		}
	}
	ImGui::EndChild();
	ImGui::PopStyleVar();

	// Done with icons.
	ImGui::Columns(1);

	// Open popup.
	if (addFolder)
	{
		ImGui::OpenPopup("Add Folder##AFPOPUP");
		addFolder = false;
	}
	// Write to popup.
	if (ImGui::BeginPopup("AddFolderPopup", ImGuiPopupFlags_NoOpenOverExistingPopup))
	{
		if (ImGui::Button("Add"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void AssetExplorer::end()
{
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	ImGui::PopStyleColor();
	ImGui::End();
}

void AssetExplorer::loadDirectories() 
{
	m_directories.clear();
	for (auto& p : std::filesystem::directory_iterator(m_currentDirectory))
	{
		// Check for file extensions.
		bool shouldDisplay = false;
		std::string extension = p.path().extension().string();
		// Do not dispay these files.
		if (p.path().filename().string()[0] == '.')
		{
			continue;
		}
		// We always want to render folders.
		else if (!extension.size())
		{
			shouldDisplay = true;
		}
		// Check if the file type should be rendred.
		else if (m_displayComponents && extension == ".lmcp")
		{
			shouldDisplay = true;
		}
		else if (m_displayCircuits && extension == ".lmct")
		{
			shouldDisplay = true;
		}
		// If no flags are set we want to display.
		else if (!m_displayCircuits && !m_displayComponents)
		{
			shouldDisplay = true;
		}

		if (!shouldDisplay) continue;

		m_directories.push_back(p);
	}
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//