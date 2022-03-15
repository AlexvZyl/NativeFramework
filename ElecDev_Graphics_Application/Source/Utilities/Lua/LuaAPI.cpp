//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Utilities/Lua/LuaInterpreter.h"
#include "Utilities/Lua/LuaAPI.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "OpenGL/SceneGL.h"
#include "Application/Application.h"
#include "Engines/Base2DEngine/Base2DEngine.h"
#include "Lumen.h"

//==============================================================================================================================================//
//  2D Drawing.																																	//
//==============================================================================================================================================//

int lua_BeginScene2D(lua_State* L) 
{
	// Get variables.
	std::string name = lua_tostring(L, 1);

	// Create engine with new scene.
	Lumen::getApp().pushEngineLayer<Base2DEngine>(name);

	return 1;
}

int lua_DrawLine2D(lua_State* L) 
{
	int stackIndex = 1;
	std::vector<float> result;

	// Color.
	lua_GetTableEntriesAndPop<float>(L, result, 4);
	glm::vec4 color = {
		result[0],
		result[1],
		result[2],
		result[3]
	};

	// Thickness.
	float thickness = lua_GetNumberAndPop<float>(L);

	// Vertex 2.
	lua_GetTableEntriesAndPop<float>(L, result, 3);
	glm::vec3 vertex2 = {
		result[0],
		result[1],
		result[2]
	};

	// Vertex 1.
	lua_GetTableEntriesAndPop<float>(L, result, 3);
	glm::vec3 vertex1 = {
		result[0],
		result[1],
		result[2]
	};

	// Draw the line.
	Renderer::addLineSegment2D(vertex1, vertex2, thickness, color);

	return 1;
}

int lua_DrawQuad2D(lua_State* L) 
{
	int stackIndex = 1;
	std::vector<float> result;

	// Color.
	lua_GetTableEntriesAndPop<float>(L, result, 4);
	glm::vec4 color = {
		result[0],
		result[1],
		result[2],
		result[3]
	};

	// Vertex 2.
	lua_GetTableEntriesAndPop<float>(L, result, 3);
	glm::vec3 vertex2 = {
		result[0],
		result[1],
		result[2]
	};

	// Vertex 1.
	lua_GetTableEntriesAndPop<float>(L, result, 3);
	glm::vec3 vertex1 = {
		result[0],
		result[1],
		result[2]
	};

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

	// Draw the line.
	Renderer::addPolygon2D(vertices, color);

	return 1;
}


//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//