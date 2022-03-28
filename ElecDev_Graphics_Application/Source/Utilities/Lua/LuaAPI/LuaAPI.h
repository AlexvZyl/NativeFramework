#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "lua/Windows/include/lua.hpp"
#include "glm/glm.hpp"
#include <string>

//==============================================================================================================================================//
//  Description.																																//
//==============================================================================================================================================//

/*

	NOTE: Drawing functions should be called right after the scene is created, since that scene
		  will then be bound for the renderer.
		  All vertices should be passed as tables {x, y, z}.
	
	BeginScene2D(name)
	DrawLine2D(vertex1, vertex2, thickness, color)
	DrawQuad2D(vertex1, vertex2, color)
	DrawRotatedQuad2D(vertex1, vertex2, color, degrees):
	DrawText2D(text, position, color, scale, horizontalAlignment, verticalAlignment):
	DrawRotatedText2D(text, position, color, scale, horizontalAlignment, verticalAlignment, degrees):
	DrawCircle2D(center, radius, color, thickness, fade):
	DrawTriangle2D(vertex1, vertex2, vertex3, color):

*/

//==============================================================================================================================================//
//  2D Rendering.																																//
//==============================================================================================================================================//

// FOR INTERNAL USE ONLY!  
// If you are using the Lua API you do not need to worry about this section.

int lua_BeginScene2D(lua_State* L);
int lua_DrawLine2D(lua_State* L);
int lua_DrawQuad2D(lua_State* L);
int lua_DrawRotatedQuad2D(lua_State* L);
int lua_DrawText2D(lua_State* L);
int lua_DrawRotatedText2D(lua_State* L);
int lua_DrawCircle2D(lua_State* L);
int lua_DrawTriangle2D(lua_State* L);

//==============================================================================================================================================//
//  ImGui.																																		//
//==============================================================================================================================================//

int lua_imgui_Text(lua_State* L);
int lua_imgui_Button(lua_State* L);
int lua_imgui_SameLine(lua_State* L);
int lua_imgui_Separator(lua_State* L);
int lua_imgui_Combo(lua_State* L);


//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
