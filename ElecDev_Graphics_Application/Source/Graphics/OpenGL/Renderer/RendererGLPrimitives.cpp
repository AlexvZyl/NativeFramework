//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "glad/glad.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "OpenGL/SceneGL.h"
#include "OpenGL/ErrorHandlerGL.h"
#include "OpenGL/Primitives/Polygon.h"
#include "OpenGL/Primitives/PolyLine.h"
#include "OpenGL/Primitives/TextureGL.h"
#include "OpenGL/Primitives/Circle.h"
#include "OpenGL/Primitives/LineSegment.h"
#include "OpenGL/Primitives/Text.h"
#include "OpenGL/Primitives/Primitive.h"
#include "OpenGL/Primitives/Vertex.h"
#include "Graphics/Entities/EntityManager.h"

//==============================================================================================================================================//
//  Removing 2D Primitives.																														//
//==============================================================================================================================================//

void Renderer::remove(PrimitivePtr* primitive)
{
	s_scene->m_primitives.erase(primitive->m_entityID);
}

//==============================================================================================================================================//
//  Adding 2D Primitives.																														//
//==============================================================================================================================================//

Polygon2D* Renderer::addPolygon2D(const std::vector<glm::vec3>& vertices, Entity* parent)
{
	unsigned id = EntityManager::peakNextID();
	s_scene->m_primitives.insert({ id, std::make_unique<Polygon2D>(vertices, s_scene->m_trianglesVAO.get(), parent) });
	return dynamic_cast<Polygon2D*>(s_scene->m_primitives.at(id).get());
}

Polygon2D* Renderer::addPolygon2D(const std::vector<glm::vec3>& vertices, const glm::vec4& color, Entity* parent)
{
	Polygon2D* polygon = Renderer::addPolygon2D(vertices, parent);
	polygon->setColor(color);
	return polygon;
}

Polygon2D* Renderer::addPolygon2DClear(const std::vector<glm::vec3>& vertices, Entity* parent)
{
	unsigned id = EntityManager::peakNextID();
	s_scene->m_primitives.insert({ id, std::make_unique<Polygon2D>(vertices, s_scene->m_linesVAO.get(), parent) });
	return dynamic_cast<Polygon2D*>(s_scene->m_primitives.at(id).get());
}

Circle* Renderer::addCircle2D(const glm::vec3& center, float radius, const glm::vec4& color, float thickness, float fade, Entity* parent)
{
	unsigned id = EntityManager::peakNextID();
	s_scene->m_primitives.insert({ id, std::make_unique<Circle>(s_scene->m_circlesVAO.get(), center, radius, color, thickness, fade, parent) });
	return dynamic_cast<Circle*>(s_scene->m_primitives.at(id).get());
}

Circle* Renderer::addCircle2D(const glm::vec2& center, float radius, const glm::vec4& color, float thickness, float fade, Entity* parent)
{
	unsigned id = EntityManager::peakNextID();
	s_scene->m_primitives.insert({ id, std::make_unique<Circle>(s_scene->m_circlesVAO.get(), center, radius, color, thickness, fade, parent) });
	return dynamic_cast<Circle*>(s_scene->m_primitives.at(id).get());
}

LineSegment* Renderer::addLineSegment2D(const glm::vec2& start, const glm::vec2& end, float thickness, const glm::vec4& colour, Entity* parent)
{
	unsigned id = EntityManager::peakNextID();
	s_scene->m_primitives.insert({ id, std::make_unique<LineSegment>(start, end, s_scene->m_trianglesVAO.get(), parent, thickness, colour) });
	return dynamic_cast<LineSegment*>(s_scene->m_primitives.at(id).get());
}

Text* Renderer::addText2D(const std::string& text, const glm::vec3& position, const glm::vec4& color, float scale, const std::string& horizontalAlignment, const std::string& verticalAlignment, Entity* parent)
{
	unsigned id = EntityManager::peakNextID();
	s_scene->m_primitives.insert({ id, std::make_unique<Text>(text, position, color, scale,
															  s_scene->m_texturedTrianglesVAO.get(), s_defaultFont.get(),
															  parent, horizontalAlignment, verticalAlignment) });
	return dynamic_cast<Text*>(s_scene->m_primitives.at(id).get());
}

PolyLine* Renderer::addPolyLine(const std::vector<glm::vec2>& vertices, Entity* parent)
{
	unsigned id = EntityManager::peakNextID();
	s_scene->m_primitives.insert({ id, std::make_unique<PolyLine>(vertices, s_scene->m_trianglesVAO.get(), parent) });
	return dynamic_cast<PolyLine*>(s_scene->m_primitives.at(id).get());
}

//==============================================================================================================================================//
//  Adding 2D Primitives from YAML.																												//
//==============================================================================================================================================//

Text* Renderer::addText2D(const YAML::Node& node, Entity* parent)
{
	Text* text = Renderer::addText2D(
		node["String"].as<std::string>(),
		{ node["Position"][0].as<float>(), node["Position"][1].as<float>(), node["Position"][2].as<float>() },
		{ node["Color"][0].as<float>(), node["Color"][1].as<float>(), node["Color"][2].as<float>(),  node["Color"][3].as<float>() },
		node["Scale"].as<float>(),
		node["Horizontal Alignment"].as<std::string>(),
		node["Vertical Alignment"].as<std::string>(),
		parent
	);

	text->setBoxColour({ node["Box Color"][0].as<float>(), node["Box Color"][1].as<float>(), node["Box Color"][2].as<float>(),  node["Box Color"][3].as<float>() });

	return text;
}

LineSegment* Renderer::addLineSegment2D(const YAML::Node& node, Entity* parent) 
{
	return Renderer::addLineSegment2D(
		{ node["Start"][0].as<float>(), node["Start"][1].as<float>() },
		{ node["End"][0].as<float>(), node["End"][1].as<float>() },
		node["Thickness"].as<float>(),
		{ node["Color"][0].as<float>(), node["Color"][1].as<float>(), node["Color"][2].as<float>(), node["Color"][3].as<float>() },
		parent
	);
}

Polygon2D* Renderer::addPolygon2D(const YAML::Node& node, Entity* parent) 
{
	// Get vertices.
	std::vector<glm::vec3> vertices;
	for (const auto& vertexNode : node["Vertices"])
	{
		YAML::Node vertex = vertexNode.second;
		vertices.push_back({ vertex[0].as<float>(), vertex[1].as<float>(), vertex[2].as<float>() });
	}

	return Renderer::addPolygon2D(
		vertices,
		{ node["Color"][0].as<float>(), node["Color"][1].as<float>(), node["Color"][2].as<float>(), node["Color"][3].as<float>() },
		parent
	);
}

Circle* Renderer::addCircle2D(const YAML::Node& node, Entity* parent) 
{
	return Renderer::addCircle2D(
		{ node["Center"][0].as<float>(), node["Center"][1].as<float>() },
		node["Radius"].as<float>(),
		{ node["Color"][0].as<float>(),  node["Color"][1].as<float>(), node["Color"][2].as<float>(), node["Color"][3].as<float>() },
		node["Thickness"].as<float>(),
		node["Fade"].as<float>(),
		parent
	);
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//