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
	lua_register(L, "BeginScene2D", lua_BeginScene2D);
	lua_register(L, "DrawLine2D",	lua_DrawLine2D);
	lua_register(L, "DrawQuad2D",	lua_DrawQuad2D);
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

	// Notifications.
	if (msg == LUA_OK)
	{
		app.pushNotification(NotificationType::Success, 5000, "Script executed.", "Lua Interpreter");
		LUMEN_LOG_SUCCESS("Script executed.", "LUA");
	}
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
inline void lua_GetTableEntriesAndPop(lua_State* L, std::vector<T>& result, int tableSize)
{
	result.clear();
	result.reserve(tableSize);
	// Get the table entries.
	for(int i = 1; i < tableSize+1; i++)
	{
		lua_pushinteger(L, i);						
		lua_gettable(L, -2);
		result.push_back((T)lua_tonumber(L, -1));
		lua_pop(L, 1);
	}
	// Pop the table from the stack.
	lua_pop(L, 1);
}

template <typename T>
T lua_GetNumberAndPop(lua_State* L) 
{
	T result = (T)lua_tonumber(L, -1);
	lua_pop(L, 1);
	return result;
}


//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//