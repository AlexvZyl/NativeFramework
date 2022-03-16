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

//==============================================================================================================================================//
//  Functions.																																	//
//==============================================================================================================================================//

inline void lua_LoadLumenFunctions(lua_State* L)
{
	lua_register(L, "BeginScene2D",			lua_BeginScene2D);
	lua_register(L, "DrawLine2D",			lua_DrawLine2D);
	lua_register(L, "DrawQuad2D",			lua_DrawQuad2D);
	lua_register(L, "DrawRotatedQuad2D",	lua_DrawRotatedQuad2D);
	lua_register(L, "DrawText2D",			lua_DrawText2D);
	lua_register(L, "DrawRotatedText2D",	lua_DrawRotatedText2D);
	lua_register(L, "DrawCircle2D",			lua_DrawCircle2D);
	lua_register(L, "DrawTriangle2D",		lua_DrawTriangle2D);
}

inline void lua_ExecuteScript(const std::string& script) 
{
	Application& app = Lumen::getApp();

	// Create Lua VM.
	lua_State* L = luaL_newstate();
	lua_LoadLumenFunctions(L);
	luaL_openlibs(L);

	// Execute the code.
	int msg = luaL_dostring(L, script.c_str());

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

// NOTE: The table has to be on the top.
template <typename T>
inline void lua_GetTableAndPop(lua_State* L, T* data, int dataSize)
{
	// Get the table entries.
	for(int i = 0; i < dataSize; i++)
	{
		lua_pushinteger(L, i+1);						
		lua_gettable(L, -2);
		data[i] = (T)lua_tonumber(L, -1);
		lua_pop(L, 1);
	}
	// Pop the table from the stack.
	lua_pop(L, 1);
}

template <typename T>
inline T lua_GetNumberAndPop(lua_State* L) 
{
	T result = (T)lua_tonumber(L, -1);
	lua_pop(L, 1);
	return result;
}

inline std::string lua_GetStringAndPop(lua_State* L)
{
	std::string result = lua_tostring(L, -1);
	lua_pop(L, 1);
	return result;
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//