#pragma once
#include "Application/LumenWindow/LumenPopupWindow.h"
#include "Graphics/OpenGL/Primitives/Text.h"
class TextEntryGUI :
    public LumenPopupWindow
{
private:
	 std::string m_textToEdit;
	 Text* m_text;
public:

	// Constructor.
	TextEntryGUI(const std::string& name, Text* text, int imguiWindowFlags = 0);

	// Rendering.
	virtual void onImGuiRender() override;
};

