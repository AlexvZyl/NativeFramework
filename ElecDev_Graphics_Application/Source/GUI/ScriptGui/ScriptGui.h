#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Application/LumenWindow/LumenWebsocketWindow.h"

//==============================================================================================================================================//
//  Forward declerations.																														//
//==============================================================================================================================================//

struct lua_State;

//==============================================================================================================================================//
//  Script GUI.																																	//
//==============================================================================================================================================//

class ScriptGui : public LumenWebsocketWindow
{
public:

	// Constructor.
	ScriptGui(const std::string& name, int windowFlags = 0);
	// Destructor.
	~ScriptGui();

	// Rendering.
	virtual void onImGuiBegin() override;
	virtual void onImGuiRender() override;
	virtual void onImGuiEnd() override;

	// Set the script that defines the gui.
	void setSctipt(std::string script);

	// Tells the GUI to get a new script.
	void awaitNewScript();

	// The Lua Script that defines the GUI.
	std::string m_script;

private:

	// Lua VM that the script is executed in.
	lua_State* m_luaState = nullptr;

	// Thread used for listening.
	std::thread m_listenerThread;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//