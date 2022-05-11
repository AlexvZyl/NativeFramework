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
#include "GUI/LumenPayload/LumenPayload.h"
#include "Engines/CircuitDesigner/GUI/CircuitEditorPopup.h"

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
	CircuitDesigner::s_engineUsedByCircuitEditor = engine;

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
			for (auto& [name, node] : engine->m_circuit->m_referenceComponents)
			{
				ImGui::PushID(componentCount++);
				// Component image.
				if (ImGui::ImageButton((void*)s_componentFileIcon, { iconSize, iconSize }, { 0, 1 }, { 1, 0 }))
				{
					app.viewComponent(node);
				}
				if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) 
				{
					auto* popup = app.pushWindow<CircuitEditorPopup>(LumenDockPanel::Floating, "");
					popup->setComponent(name);
				}

				// Drag & drop.
				LumenPayload payload(LumenPayloadType::YamlNode);
				payload.setDragAndDropSource(node);

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

		// Drag & drop.
		LumenPayload payloadComp(LumenPayloadType::String);
		payloadComp.setDragAndDropTarget();
		if (payloadComp.hasValidData()) engine->importComponent(payloadComp.getDataString(), false);

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
			for (auto& [name, node] : engine->m_circuit->m_referenceCables)
			{
				ImGui::PushID(cableCount++);
				if (ImGui::ImageButton((void*)s_cableIcon, { iconSize, iconSize }, { 0, 1 }, { 1, 0 }))
				{

				}
				if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
				{
					auto* popup = app.pushWindow<CircuitEditorPopup>(LumenDockPanel::Floating, "");
					popup->setCable(name);
				}

				// Drag & drop.
				LumenPayload payload(LumenPayloadType::YamlNode);
				payload.setDragAndDropSource(node);

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

		// Drag & drop.
		LumenPayload payloadCable(LumenPayloadType::String);
		payloadCable.setDragAndDropTarget();
		if (payloadCable.hasValidData()) engine->importCable(payloadCable.getDataString());

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