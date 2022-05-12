#include "TextEntryGUI.h"

TextEntryGUI::TextEntryGUI(const std::string& name, Text* text, int imguiWindowFlags) : LumenPopupWindow(name, imguiWindowFlags), m_text(text)
{
	removeImGuiWindowFlags(ImGuiWindowFlags_NoResize);
	addImGuiWindowFlags(ImGuiWindowFlags_AlwaysAutoResize);
}

void TextEntryGUI::onImGuiRender()
{
	ImGui::SetKeyboardFocusHere();
	if (ImGui::InputText("##Text", &m_textToEdit, ImGuiInputTextFlags_AlwaysOverwrite)) {
		m_text->updateText(m_textToEdit);
	}
	if (ImGui::IsKeyPressed(ImGuiKey_Enter)) closeWindow();
}
