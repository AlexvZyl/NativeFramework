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
}

void AssetExplorer::begin()
{
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0.f, 10.f});
	ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 10.f);
	ImGui::PushStyleColor(ImGuiCol_Button, {0.f, 0.f, 0.f, 0.f});
	ImGui::Begin(m_name.c_str(), NULL, m_imguiWindowFlags);
}

void AssetExplorer::onRender()
{
	// TODO:  We do not have to check this each frame.
	 
	static ImGuiTextFilter filter;

	// Current directory.
	static glm::vec2 headerSize(20, 20);

	// Move up button.
	if (ImGui::ImageButton((void*)s_leftArrowIcon, headerSize))
	{
		m_currentDirectory = m_currentDirectory.parent_path();
		filter.Clear();
	}
	
	// Spacing.
	ImGui::SameLine();
	ImGui::Text("  ");
	ImGui::SameLine();

	// TODO:  Add to prevent loading files every frame.
	//// Reload button.
	//if (ImGui::ImageButton((void*)s_reloadIcon, headerSize))
	//{
	//}
	//// Spacing.
	//ImGui::SameLine();
	//ImGui::Text("  ");
	//ImGui::SameLine();

	// Current directory button.
	if(ImGui::Button(m_currentDirectory.string().c_str(), {0, headerSize.y + 7.f}))
	{
		std::string newDirectory = selectFolder(m_currentDirectory.string());
		if (newDirectory.size())
		{
			m_currentDirectory = newDirectory;
			filter.Clear();
		}
	}

	// Filter.
	ImGui::SameLine();
	float filterSize = 400;
	ImGui::SetCursorPosX(m_contentRegionSize.x - filterSize);
	ImGui::Text("Search: ");
	ImGui::SameLine();
	filter.Draw("##AssetExplorerSearch", filterSize);
	ImGui::Separator();

	// Create icon columns.
	float iconSize = 90;
	float padding = 7;
	float cellSize = iconSize + 2 * padding;
	int columns = std::floor(m_contentRegionSize.x * 0.9 / cellSize);
	if (columns <= 0) columns = 1;
	ImGui::Columns(columns, 0, false);

	// Display contents.
	int directoryCount = 0;
	for (auto& p : std::filesystem::directory_iterator(m_currentDirectory))
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
				filter.Clear();
			}
			ImGui::TextWrapped(p.path().filename().string().c_str());
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

			// The button label applied to all icons.
			ImGui::TextWrapped(p.path().filename().string().c_str());
		}
		ImGui::NextColumn();
		ImGui::PopID();
	}

	// Done with icons.
	ImGui::Columns(1);
}

void AssetExplorer::end()
{
	ImGui::End();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	ImGui::PopStyleColor();
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//