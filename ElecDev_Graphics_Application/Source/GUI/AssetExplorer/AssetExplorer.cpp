//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Application/Application.h"
#include "AssetExplorer.h"
#include "Lumen.h"
#include "Utilities/Windows/WindowsUtilities.h"
#include "Resources/ResourceHandler.h"

//==============================================================================================================================================//
//  Statics.																																	//
//==============================================================================================================================================//

std::string AssetExplorer::s_startingDirectory = getExecutableLocation(true);
std::filesystem::path AssetExplorer::m_currentDirectory;
unsigned AssetExplorer::s_fileIcon = NULL;
unsigned AssetExplorer::s_folderIcon = NULL;
unsigned AssetExplorer::s_circuitFileIcon = NULL;
unsigned AssetExplorer::s_leftArrowIcon = NULL;
unsigned AssetExplorer::s_componentFileIcon = NULL;
unsigned AssetExplorer::s_reloadIcon = NULL;

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
	loadDirectories();
}

void AssetExplorer::begin()
{
	ImGui::Begin(m_name.c_str(), NULL, m_imguiWindowFlags);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0.f, 10.f});
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

	// Header size.
	static glm::vec2 headerSize(20, 20);

	// Move up button.
	if (ImGui::ImageButton((void*)s_leftArrowIcon, headerSize))
	{
		m_currentDirectory = m_currentDirectory.parent_path();
		m_clearFilterOnFrameStart = true;
		loadDirectories();
	}
	
	// Spacing.
	ImGui::SameLine();
	ImGui::Text("  ");
	ImGui::SameLine();

	// Reload button.
	if (ImGui::ImageButton((void*)s_reloadIcon, headerSize))
	{
		loadDirectories();
	}

	// Spacing.
	ImGui::SameLine();
	ImGui::Text("  ");
	ImGui::SameLine();

	// Current directory button.
	if(ImGui::Button(m_currentDirectory.string().c_str(), {0, headerSize.y + 7.f}))
	{
		std::string newDirectory = selectFolder(m_currentDirectory.string());
		if (newDirectory.size())
		{
			m_currentDirectory = newDirectory;
			m_clearFilterOnFrameStart = true;
			loadDirectories();
		}
	}

	// Filter.
	ImGui::SameLine();
	float filterSize = 250;
	ImGui::SetCursorPosX(m_contentRegionSize.x - filterSize);
	ImGui::Text("Search: ");
	ImGui::SameLine();
	filter.Draw("##AssetExplorerSearch", filterSize);
	ImGui::Separator();

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
		if (!filter.PassFilter(p.path().stem().string().c_str())) continue;;

		ImGui::PushID(directoryCount++);

		// Directories.
		if (p.is_directory())
		{
			ImGui::ImageButton((void*)s_folderIcon, {iconSize, iconSize}, { 0, 1 }, { 1, 0 });
			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered()) 
			{
				m_currentDirectory /= p.path().filename();
				m_clearFilterOnFrameStart = true;
				loadDirectories();
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
					std::string item = p.path().string();
					FileDropEvent event(item);
					Lumen::getApp().logEvent<FileDropEvent>(event);
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

	// Done with icons.
	ImGui::Columns(1);
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
		m_directories.push_back(p);
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//