//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Utilities/Lua/LuaInterpreter.h"
#include "Utilities/Lua/LuaAPI/LuaAPI.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "OpenGL/SceneGL.h"
#include "OpenGL/Primitives/Polygon.h"
#include "OpenGL/Primitives/Text.h"
#include "Application/Application.h"
#include "Engines/Base2DEngine/Base2DEngine.h"
#include "Lumen.h"

//==============================================================================================================================================//
//  Scenes.																																		//
//==============================================================================================================================================//

int lua_BeginScene2D(lua_State* L) 
{
	// Get data.
	std::string name = lua_tostring(L, 1);

	// Execute function.
	Lumen::getApp().pushEngine<Base2DEngine>(LumenDockPanel::Scene, name);

	return 1;
}

//==============================================================================================================================================//
//  Lines.																																		//
//==============================================================================================================================================//

int lua_DrawLine2D(lua_State* L) 
{
	// Get data.
	glm::vec4 color(0.f);
	glm::vec3 vertex2(0.f);
	glm::vec3 vertex1(0.f);
	lua_GetTableAndPop<float>(L, &color[0], 4);
	float thickness = lua_GetNumberAndPop<float>(L);
	lua_GetTableAndPop<float>(L, &vertex2[0], 3);
	lua_GetTableAndPop<float>(L, &vertex1[0], 3);

	// Draw.
	Renderer::addLineSegment2D(vertex1, vertex2, thickness, color);

	return 1;
}

//==============================================================================================================================================//
//  Quads.																																		//
//==============================================================================================================================================//

int lua_DrawQuad2D(lua_State* L) 
{
	// Get data.
	glm::vec4 color(0.f);
	glm::vec3 vertex2(0.f);
	glm::vec3 vertex1(0.f);
	lua_GetTableAndPop<float>(L, &color[0], 4);
	lua_GetTableAndPop<float>(L, &vertex2[0], 3);
	lua_GetTableAndPop<float>(L, &vertex1[0], 3);

	// Construct the vertices.
	std::vector<glm::vec3> vertices;
	vertices.reserve(4);
	vertices.emplace_back(vertex1);
	vertices.emplace_back(glm::vec3(
		vertex1.x,
		vertex2.y,
		vertex1.z
	));
	vertices.emplace_back(vertex2);
	vertices.emplace_back(glm::vec3(
		vertex2.x,
		vertex1.y,
		vertex2.z
	));

	// Draw.
	Renderer::addPolygon2D(vertices, color);

	return 1;
}

int lua_DrawRotatedQuad2D(lua_State* L)
{
	// Get data.
	glm::vec4 color(0.f);
	glm::vec3 vertex2(0.f);
	glm::vec3 vertex1(0.f);
	float degrees = lua_GetNumberAndPop<float>(L);
	lua_GetTableAndPop<float>(L, &color[0], 4);
	lua_GetTableAndPop<float>(L, &vertex2[0], 3);
	lua_GetTableAndPop<float>(L, &vertex1[0], 3);

	// Construct the vertices.
	std::vector<glm::vec3> vertices;
	vertices.reserve(4);
	vertices.emplace_back(vertex1);
	vertices.emplace_back(glm::vec3(
		vertex1.x,
		vertex2.y,
		vertex1.z
	));
	vertices.emplace_back(vertex2);
	vertices.emplace_back(glm::vec3(
		vertex2.x,
		vertex1.y,
		vertex2.z
	));

	// Draw.
	Polygon2D* poly =  Renderer::addPolygon2D(vertices, color);
	poly->rotate(degrees);

	return 1;
}

//==============================================================================================================================================//
//  Text.																																		//
//==============================================================================================================================================//

int lua_DrawText2D(lua_State* L) 
{
	// Get data.
	glm::vec4 color(0.f);
	glm::vec3 position(0.f);
	std::string verticalAlignment = lua_GetStringAndPop(L);
	std::string horizontalAlignment = lua_GetStringAndPop(L);
	float scale = lua_GetNumberAndPop<float>(L);
	lua_GetTableAndPop<float>(L, &color[0], 4);
	lua_GetTableAndPop<float>(L, &position[0], 3);
	std::string text = lua_GetStringAndPop(L);

	// Draw.
	Renderer::addText2D(text, position, color, scale, horizontalAlignment, verticalAlignment);

	return 1;
}

int lua_DrawRotatedText2D(lua_State* L) 
{
	// Get data.
	glm::vec4 color(0.f);
	glm::vec3 position(0.f);
	float degrees = lua_GetNumberAndPop<float>(L);
	std::string verticalAlignment = lua_GetStringAndPop(L);
	std::string horizontalAlignment = lua_GetStringAndPop(L);
	float scale = lua_GetNumberAndPop<float>(L);
	lua_GetTableAndPop<float>(L, &color[0], 4);
	lua_GetTableAndPop<float>(L, &position[0], 3);
	std::string text = lua_GetStringAndPop(L);

	// Draw.
	Text* textEnt = Renderer::addText2D(text, position, color, scale, horizontalAlignment, verticalAlignment);
	textEnt->rotate(degrees);

	return 1;
}

//==============================================================================================================================================//
//  Circles.																																	//
//==============================================================================================================================================//

int lua_DrawCircle2D(lua_State* L)
{
	// Get data.
	glm::vec4 color(0.f);
	glm::vec3 center(0.f);
	float fade = lua_GetNumberAndPop<float>(L);
	float thickness = lua_GetNumberAndPop<float>(L);
	lua_GetTableAndPop<float>(L, &color[0], 4);
	float radius = lua_GetNumberAndPop<float>(L);
	lua_GetTableAndPop<float>(L, &center[0], 3);

	// Draw.
	Renderer::addCircle2D(center, radius, color, thickness, fade);

	return 1;
}

//==============================================================================================================================================//
//  Triangles.																																	//
//==============================================================================================================================================//

int lua_DrawTriangle2D(lua_State* L)
{
	// Get data.
	glm::vec4 color(0.f);
	glm::vec3 vertex3(0.f); 
	glm::vec3 vertex2(0.f);
	glm::vec3 vertex1(0.f);
	lua_GetTableAndPop<float>(L, &color[0], 4);
	lua_GetTableAndPop<float>(L, &vertex3[0], 3);
	lua_GetTableAndPop<float>(L, &vertex2[0], 3);
	lua_GetTableAndPop<float>(L, &vertex1[0], 3);

	// Construct vertices.
	std::vector<glm::vec3> vertices;
	vertices.reserve(3);
	vertices.push_back(vertex1);
	vertices.push_back(vertex2);
	vertices.push_back(vertex3);

	// Draw.
	Renderer::addPolygon2D(vertices, color);
	return 1;
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//