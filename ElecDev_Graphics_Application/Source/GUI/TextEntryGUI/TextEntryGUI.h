#pragma once

#include "Application/Windows/LumenPopupWindow.h"
#include "Graphics/OpenGL/Primitives/Text.h"

class CommandLog;

class TextEntryGUI : public LumenPopupWindow
{

public:

	// Constructor.
	TextEntryGUI(const std::string& name, Text* text, CommandLog* log, int imguiWindowFlags = 0);

	// Rendering.
	virtual void onImGuiRender() override;

private:

	Text* m_text;
	CommandLog* m_log;
	std::string strBeforeEdit;
};

