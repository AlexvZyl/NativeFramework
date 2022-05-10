//=======================================================================================================================================//
// Includes.																															 //
//=======================================================================================================================================//

#include "CircuitEditor.h"
#include "Lumen.h"
#include "Application/Application.h"
#include "Engines/CircuitDesigner/CircuitDesigner.h"
#include "Engines/CircuitDesigner/Peripherals/Circuit.h"
#include "Engines/AssetViewer/AssetViewer.h"
#include "Resources/ResourceHandler.h"

//=======================================================================================================================================//
// Circuit editor.																														 //
//=======================================================================================================================================//

CircuitEditor::CircuitEditor(std::string name, int windowFlags)
	: LumenWindow(name, windowFlags)
{
	addImGuiWindowFlags(ImGuiWindowFlags_AlwaysAutoResize);

	s_componentFileIcon = loadBitmapToGL(loadImageFromResource(COMPONENT_FILE_ICON));
	s_cableIcon = loadBitmapToGL(loadImageFromResource(CABLE_ICON));
}

void CircuitEditor::onImGuiBegin() 
{
	ImGui::Begin(getImGuiName(), &m_isOpen, getImGuiWindowFlags());
}

void CircuitEditor::onImGuiRender() 
{
	Application& app = Lumen::getApp();
	CircuitDesigner* engine = app.getActiveEngine<CircuitDesigner>();
	AssetViewer* assetViewerEngine = app.getAssetViewerEngine();

	if(engine)
	{
		// Circuit name.
		ImGui::Text("Circuit Name: ");
		ImGui::SameLine();
		if (ImGui::InputText("##circuitName", &engine->m_circuit->m_label))
		{
			engine->setName(engine->m_circuit->m_label);
		}

		// Setup style.
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 5.f, 5.f });
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.f, 0.f, 0.f, 0.f });

		// --------------------- //
		//  C O M P O N E N T S  //
		// --------------------- //
		
		// Display components.
		ImGui::Text("Components:");
		if (ImGui::BeginChild("ImportedComponents", {0.f, m_contentRegionSize.y/2.f}, true))
		{
			// Setup columns.
			float iconSize = 75;
			float padding = 7;
			float cellSize = iconSize + 2 * padding;
			int columns = std::floor(m_contentRegionSize.x * 0.9 / cellSize);
			if (columns <= 0) columns = 1;
			ImGui::Columns(columns, 0, false);

			// Iterate and display components.
			int componentCount = 0;
			for (auto& [name, node] : engine->m_circuit->m_uniqueComponents)
			{
				ImGui::PushID(componentCount++);
				// Component image.
				if (ImGui::ImageButton((void*)s_componentFileIcon, { iconSize, iconSize }, { 0, 1 }, { 1, 0 }))
				{
					app.viewComponent(node);
				}

				// File drag & drop.
				if (ImGui::BeginDragDropSource())
				{
					//const wchar_t* itemPath = p.path().c_str();
					//ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t), ImGuiCond_Once);
					ImGui::EndDragDropSource();
				}

				// Component name.
				glm::vec2 stringSize = ImGui::CalcTextSize(name.c_str());
				if (stringSize.x < cellSize)
					ImGui::SetCursorPosX(ImGui::GetCursorPos().x + (cellSize / 2) - (stringSize.x / 2));
				ImGui::TextWrapped(name.c_str());

				ImGui::NextColumn();
				ImGui::PopID();
			}
		}
		ImGui::EndChild();

		// Receive dropped files.
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				// Pass FileDropEvent to engine.
				const wchar_t* path = (const wchar_t*)payload->Data;
				std::wstring wPath(path);
				std::string sPath;
				sPath.insert(sPath.end(), wPath.begin(), wPath.end());
				engine->importComponent(sPath);
			}
			ImGui::EndDragDropTarget();
		}

		// ------------- //
		//  C A B L E S  //
		// ------------- //

		// Display cables.
		ImGui::Text("Cables:");
		if (ImGui::BeginChild("ImportedCables", { 0.f, 0.f }, true))
		{
			// Setup columns.
			float iconSize = 75;
			float padding = 7;
			float cellSize = iconSize + 2 * padding;
			int columns = std::floor(m_contentRegionSize.x * 0.9 / cellSize);
			if (columns <= 0) columns = 1;
			ImGui::Columns(columns, 0, false);

			// Iterate and display the cables.
			int cableCount = 0;
			for (auto& [name, node] : engine->m_circuit->m_uniqueCables)
			{
				ImGui::PushID(cableCount++);
				if (ImGui::ImageButton((void*)s_cableIcon, { iconSize, iconSize }, { 0, 1 }, { 1, 0 }))
				{

				}

				// File drag & drop.
				if (ImGui::BeginDragDropSource())
				{
					/*const wchar_t* itemPath = p.path().c_str();
					ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t), ImGuiCond_Once);*/
					ImGui::EndDragDropSource();
				}

				// Component name.
				glm::vec2 stringSize = ImGui::CalcTextSize(name.c_str());
				if (stringSize.x < cellSize)
					ImGui::SetCursorPosX(ImGui::GetCursorPos().x + (cellSize / 2) - (stringSize.x / 2));
				ImGui::TextWrapped(name.c_str());

				ImGui::NextColumn();
				ImGui::PopID();
			}
		}
		ImGui::EndChild();

		// Receive dropped files.
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				std::wstring wPath(path);
				std::string sPath;
				sPath.insert(sPath.end(), wPath.begin(), wPath.end());
				engine->importCable(sPath);
			}
			ImGui::EndDragDropTarget();
		}

		// Done with style.
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
	}

	else 
	{
		ImGui::Text("No active circuit.");
	}
}

void CircuitEditor::onImGuiEnd() 
{
	ImGui::End();
}

//=======================================================================================================================================//
// EOF.																																	 //
//=======================================================================================================================================//