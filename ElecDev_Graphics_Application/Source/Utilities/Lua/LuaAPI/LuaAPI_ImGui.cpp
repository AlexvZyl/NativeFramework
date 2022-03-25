//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "GUI/ScriptGui/ScriptGui.h"
#include "Utilities/Lua/LuaInterpreter.h"
#include "Utilities/Lua/LuaAPI/LuaAPI.h"
#include "imgui/imgui.h"

//==============================================================================================================================================//
//  Functions.																																	//
//==============================================================================================================================================//

int lua_imgui_Text(lua_State* L) 
{
	// Get data.
	std::string text = lua_GetStringAndPop(L);

	// Render.
	ImGui::Text(text.c_str());

	return 1;
}

int lua_imgui_Button(lua_State* L)
{
	// Get data.
	glm::vec2 size(0.f);
	lua_GetTableAndPop<float>(L, &size[0], 2);
	std::string text = lua_GetStringAndPop(L);

	// Render.
	if (ImGui::Button(text.c_str(), size))
	{
		// Press occurred.
		std::string msg = "[Button] " + text + " : Pressed.";
		Lumen::getActiveScriptGui()->callbackMessage(msg);
		return 1;
	}

	// Press did not occur.
	return 0;
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//