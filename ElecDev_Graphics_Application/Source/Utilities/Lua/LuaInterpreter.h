#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <vector>
#include <string>
#include "Utilities/Lua/LuaAPI.h"
#include "lua/Windows/include/lua.hpp"
#include "Application/Application.h"
#include "Utilities/Logger/Logger.h"
#include "Utilities/Assert/Assert.h"
#include <map>

//==============================================================================================================================================//
//  Functions.																																	//
//==============================================================================================================================================//

inline void lua_LoadLumenFunctions(lua_State* L)
{
	// 2D Drawing.
	lua_register(L, "BeginScene2D",				lua_BeginScene2D);
	lua_register(L, "DrawLine2D",				lua_DrawLine2D);
	lua_register(L, "DrawQuad2D",				lua_DrawQuad2D);
	lua_register(L, "DrawRotatedQuad2D",		lua_DrawRotatedQuad2D);
	lua_register(L, "DrawText2D",				lua_DrawText2D);
	lua_register(L, "DrawRotatedText2D",		lua_DrawRotatedText2D);
	lua_register(L, "DrawCircle2D",				lua_DrawCircle2D);
	lua_register(L, "DrawTriangle2D",			lua_DrawTriangle2D);

	// ImGui.
	lua_register(L, "ImGui_Text",				lua_imgui_Text);
	lua_register(L, "ImGui_Button",				lua_imgui_Button);
	lua_register(L, "ImGui_CloseButton",		lua_imgui_CloseButton);
	lua_register(L, "ImGui_ClearAwaitScriptButton",	
												lua_imgui_ClearAwaitScriptButton);
	lua_register(L, "ImGui_SameLine",			lua_imgui_SameLine);
	lua_register(L, "ImGui_Separator",			lua_imgui_Separator);
	lua_register(L, "ImGui_Combo",				lua_imgui_Combo);
	lua_register(L, "ImGui_Checkbox",			lua_imgui_Checkbox);
	lua_register(L, "ImGui_InputText",			lua_imgui_InputText);
	lua_register(L, "ImGui_Table",				lua_imgui_Table);

	// GUI's.
	lua_register(L, "Gui_Bucket",				lua_gui_Bucket);
	lua_register(L, "Gui_Transformer",			lua_gui_Transformer);
}

inline lua_State* lua_CreateNewLuaState() 
{
	// Create Lua VM.
	lua_State* L = luaL_newstate();
	lua_LoadLumenFunctions(L);
	luaL_openlibs(L);
	return L;
}

inline void lua_ExecuteScript(lua_State* L, const std::string& script, bool notify = false)
{
	// Execute the code.
	int msg = luaL_dostring(L, script.c_str());

	// Do not notify.
	if (!notify) return;

	Application& app = Lumen::getApp();

	// OK.
	if (msg == LUA_OK)
	{
		app.pushNotification(NotificationType::Success, 5000, "Script executed.", "Lua Interpreter");
		LUMEN_LOG_SUCCESS("Script executed.", "LUA");
	}
	// Error.
	else 
	{
		std::string errorMsg = lua_tostring(L, -1);
		LUMEN_LOG_ERROR(errorMsg, "LUA");
	}
}

//==============================================================================================================================================//
//  Utilities.																																	//
//==============================================================================================================================================//

inline bool lua_GetBooleanAndPop(lua_State* L)
{
	LUMEN_DEBUG_ASSERT(lua_isboolean(L, -1), "Incorrect type.");
	bool result = lua_toboolean(L, -1);
	lua_pop(L, 1);
	return result;
}

template <typename T>
inline T lua_GetNumberAndPop(lua_State* L) 
{
	LUMEN_DEBUG_ASSERT(lua_isnumber(L, -1), "Incorrect type.");
	T result = (T)lua_tonumber(L, -1);
	lua_pop(L, 1);
	return result;
}

inline std::string&& lua_GetStringAndPop(lua_State* L)
{
	LUMEN_DEBUG_ASSERT(lua_isstring(L, -1), "Incorrect type.");
	std::string result = lua_tostring(L, -1);
	lua_pop(L, 1);
	return std::move(result);
}

template <typename T>
inline std::vector<T>&& lua_GetTableAndPop(lua_State* L)
{
	// Init data.
	std::vector<T> data;
	// Get data from table.
	lua_pushnil(L);
	while (lua_next(L, -2) != 0) 
		data.push_back(lua_GetNumberAndPop<T>(L));
	// Pop the table from the stack.
	lua_pop(L, 1);
	return std::move(data);
}

template <typename T>
inline void lua_GetTableAndPop(lua_State* L, T* data)
{
	// Get data from table.
	lua_pushnil(L);
	int i = 0;
	while (lua_next(L, -2) != 0)
		data[i++] = lua_GetNumberAndPop<T>(L);
	// Pop the table from the stack.
	lua_pop(L, 1);
}

inline std::vector<std::string>&& lua_GetStringTableAndPop(lua_State* L)
{
	// Init return data.
	std::vector<std::string> data;
	// Get data from table.
	lua_pushnil(L);
	while(lua_next(L, -2) != 0) 
		data.push_back(lua_GetStringAndPop(L));
	// Pop the table from the stack.
	lua_pop(L, 1);
	return std::move(data);
}

inline std::map<std::string, std::vector<std::string>>&& lua_GetDictAndPop(lua_State* L)
{
	// Init return data.
	std::map<std::string, std::vector<std::string>> data;
	// First key.
	lua_pushnil(L);
	// Get dict entries.
	while(lua_next(L, -2) != 0)
	{
		// Place values table on top.
		const char* key = lua_tostring(L, -2);
		lua_gettable(L, -2);
		// Get key & values.
		data.insert({ key, lua_GetStringTableAndPop(L) });
	}
	// Pop dict.
	lua_pop(L, 1);
	return std::move(data);
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//