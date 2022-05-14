#pragma once
#include "Application/LumenWindow/LumenPopupWindow.h"
#include "Graphics/OpenGL/Primitives/Text.h"
class TextEntryGUI :
    public LumenPopupWindow
{
private:
	Text* m_text;
	std::string m_textToEdit;
public:

	// Constructor.
	TextEntryGUI(const std::string& name, Text* text, int imguiWindowFlags = 0);

	// Rendering.
	virtual void onImGuiRender() override;
};

