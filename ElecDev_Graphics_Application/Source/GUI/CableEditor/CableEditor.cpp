//=======================================================================================================================================//
// Includes.																															 //
//=======================================================================================================================================//

#include "CableEditor.h"
#include "Lumen.h"
#include "Application/Application.h"
#include "Engines/Design2DEngine/Design2DEngine.h"
#include "Engines/Design2DEngine/Peripherals/Cable.h"
#include "Resources/ResourceHandler.h"

//=======================================================================================================================================//
// Circuit editor.																														 //
//=======================================================================================================================================//

CableEditor::CableEditor(std::string name, int windowFlags)
	: GuiElementCore(name, windowFlags)
{
	m_imguiWindowFlags |= ImGuiWindowFlags_MenuBar;

	if (!s_iconCreated)
	{
		s_cableIcon = loadBitmapToGL(loadImageFromResource(CABLE_ICON));
		s_iconCreated = true;
		s_textHeight = ImGui::CalcTextSize("A").y;
	}
}

void CableEditor::begin()
{
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
	ImGui::Begin(m_name.c_str(), &m_isOpen, m_imguiWindowFlags);
}

void CableEditor::onRender()
{
	Application& app = Lumen::getApp();
	Design2DEngine* engine = app.getActiveEngine<Design2DEngine>();

	if (engine)
	{
		Cable* cable = engine->m_activeCable.get();
		if (cable) 
		{
			if (ImGui::BeginMenuBar())
			{
				ImGui::SetCursorPosX(m_contentRegionSize.x / 2 - ImGui::CalcTextSize(cable->m_titleString.c_str()).x/2 );
				ImGui::Text(cable->m_titleString.c_str());
				ImGui::EndMenuBar();
			}

			static glm::vec2 windowPadding = {15.f, 15.f};
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, windowPadding);
			static float iconTextSeperation = 10.f;
			// Original setup.
			glm::vec2 iconSize = { 100, 100 };
			// Now adjust for the wrapped text.
			glm::vec2 textSize = ImGui::CalcTextSize(cable->m_cableType.c_str(), 0, false, iconSize.x);
			//ImGui::SetCursorPos();
			glm::vec2 childSize = { iconSize.x + 2 * windowPadding.x, iconSize.y + textSize.y + 2 * windowPadding.y + iconTextSeperation };
			ImGui::SetCursorPosX(m_contentRegionSize.x/2 - childSize.x/2);
			if (ImGui::BeginChild("CableDrop", childSize, true, ImGuiWindowFlags_NoScrollbar))
			{
				if (cable->m_cableType.size())
				{
					//ImGui::SetCursorPosX(padding.x/2);
					ImGui::Image((void*)s_cableIcon, iconSize, { 0, 1 }, { 1, 0 });
					if (textSize.x < iconSize.x)
					{
						ImGui::SetCursorPosX(iconSize.x/2 - textSize.x/2 + windowPadding.x);
					}
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + iconTextSeperation);
					ImGui::TextWrapped(cable->m_cableType.c_str());

				}
				else 
				{
					ImGui::TextWrapped("No assigned type.");
				}
			}
			ImGui::PopStyleVar();
			ImGui::EndChild();
			// Receive dropped files.
			if (ImGui::BeginDragDropTarget())
			{
				
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					// Pass FileDropEvent to engine.
					std::filesystem::path fsPath((const wchar_t*)payload->Data);
					if (fsPath.filename().extension().string() == ".lmcb")
					{
						cable->m_cableType = fsPath.filename().stem().string();
					}
				}
				ImGui::EndDragDropTarget();
			}
		}
		else 
		{
			ImGui::Text("No active cable.");
		}
	}
	else
	{
		ImGui::Text("No active engine.");
	}
}

void CableEditor::end()
{
	ImGui::End();
	ImGui::PopStyleVar();
}

//=======================================================================================================================================//
// EOF.																																	 //
//=======================================================================================================================================//