#include "TextEntryGUI.h"
#include "Engines/CommandSystem/Command.h"

TextEntryGUI::TextEntryGUI(const std::string& name, Text* text, CommandLog* log, int imguiWindowFlags) : LumenPopupWindow(name, imguiWindowFlags), m_text(text), m_log(log)
{
	removeImGuiWindowFlags(ImGuiWindowFlags_NoResize);
	addImGuiWindowFlags(ImGuiWindowFlags_AlwaysAutoResize);
}

void TextEntryGUI::onImGuiRender()
{
	ImGui::SetKeyboardFocusHere();	
	ImGui::InputText("##text", &m_text->m_string);
	if (ImGui::IsItemEdited()) {
		m_text->update();
	}
	if (ImGui::IsItemActivated()) {
		//save original data
		strBeforeEdit = m_text->m_string;
	}
	if (ImGui::IsItemDeactivatedAfterEdit()) {
		//log change
		m_log->log<EditTextCommand>(m_text, m_text->m_string, strBeforeEdit);
	}
	if (ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::IsKeyPressed(ImGuiKey_Escape)) {
		//TODO: check for whitespace, and remove text if blank
		//Perform special checks if Text field is a port name.

		closeWindow();
	}
}
