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

void Renderer::remove(IPrimitive* primitive)
{
	primitive->removeFromGraphicsBuffer();
}

//==============================================================================================================================================//
//  Adding 2D Primitives.																														//
//==============================================================================================================================================//

std::unique_ptr<Polygon2D>&& Renderer::addPolygon2D(const std::vector<glm::vec3>& vertices, Entity* parent)
{
	return std::move( std::make_unique<Polygon2D>(vertices, &s_scene->m_trianglesBuffer, parent) );
}

std::unique_ptr<Polygon2D>&& Renderer::addPolygon2D(const std::vector<glm::vec3>& vertices, const glm::vec4& color, Entity* parent)
{
	return std::move( std::make_unique<Polygon2D>(vertices, &s_scene->m_trianglesBuffer, parent, color) );
}

std::unique_ptr<PolyLine>&& Renderer::addPolygon2DClear(const std::vector<glm::vec2>& vertices, float thickness, Entity* parent, glm::vec4 colour)
{
	return std::move( std::make_unique<PolyLine>(vertices, &s_scene->m_trianglesBuffer, parent, thickness, true, colour) );
}

std::unique_ptr<Circle>&& Renderer::addCircle2D(const glm::vec3& center, float radius, const glm::vec4& color, float thickness, float fade, Entity* parent)
{
	return std::move( std::make_unique<Circle>(&s_scene->m_circlesBuffer, center, radius, color, thickness, fade, parent) );
}

std::unique_ptr<Circle>&& Renderer::addCircle2D(const glm::vec2& center, float radius, const glm::vec4& color, float thickness, float fade, Entity* parent)
{
	return std::move( std::make_unique<Circle>(&s_scene->m_circlesBuffer, center, radius, color, thickness, fade, parent) );
}

std::unique_ptr<LineSegment>&& Renderer::addLineSegment2D(const glm::vec2& start, const glm::vec2& end, float thickness, const glm::vec4& colour, Entity* parent)
{
	return std::move( std::make_unique<LineSegment>(start, end, &s_scene->m_trianglesBuffer, parent, thickness, colour) );
}

std::unique_ptr<Text>&& Renderer::addText2D(const std::string& text, const glm::vec3& position, const glm::vec4& color, float scale, const std::string& horizontalAlignment, const std::string& verticalAlignment, Entity* parent)
{
	return std::move( std::make_unique<Text>(text, position, color, scale,
										     &s_scene->m_texturedTrianglesBuffer, s_defaultFont.get(),
										     parent, horizontalAlignment, verticalAlignment) );
}

std::unique_ptr<Text>&& Renderer::addText2D(const std::string& text, const glm::vec2& position, const glm::vec4& color, float scale, const std::string& horizontalAlignment, const std::string& verticalAlignment, Entity* parent)
{
	return std::move (std::make_unique<Text>(text, glm::vec3{position, 0.f}, color, scale,
											 &s_scene->m_texturedTrianglesBuffer, s_defaultFont.get(),
											 parent, horizontalAlignment, verticalAlignment) );
}

std::unique_ptr<PolyLine>&& Renderer::addPolyLine(const std::vector<glm::vec2>& vertices, float thickness, const glm::vec4& color, bool rounded, Entity* parent)
{
	return std::move( std::make_unique<PolyLine>(vertices, &s_scene->m_trianglesBuffer, parent, thickness, false, color, rounded) );
}

//==============================================================================================================================================//
//  Adding 2D Primitives from YAML.																												//
//==============================================================================================================================================//

std::unique_ptr<Text>&& Renderer::addText2D(const YAML::Node& node, Entity* parent)
{
	auto text = Renderer::addText2D(
		node["String"].as<std::string>(),
		{ node["Position"][0].as<float>(), node["Position"][1].as<float>(), node["Position"][2].as<float>() },
		{ node["Color"][0].as<float>(), node["Color"][1].as<float>(), node["Color"][2].as<float>(),  node["Color"][3].as<float>() },
		node["Scale"].as<float>(),
		node["Horizontal Alignment"].as<std::string>(),
		node["Vertical Alignment"].as<std::string>(),
		parent
	);
	text->setBoxColour({ node["Box Color"][0].as<float>(), node["Box Color"][1].as<float>(), node["Box Color"][2].as<float>(),  node["Box Color"][3].as<float>() });
	return std::move(text);
}

std::unique_ptr<LineSegment>&& Renderer::addLineSegment2D(const YAML::Node& node, Entity* parent) 
{
	return Renderer::addLineSegment2D(
		{ node["Start"][0].as<float>(), node["Start"][1].as<float>() },
		{ node["End"][0].as<float>()  , node["End"][1].as<float>()   },
		node["Thickness"].as<float>() ,
		{ node["Color"][0].as<float>(), node["Color"][1].as<float>(), node["Color"][2].as<float>(), node["Color"][3].as<float>() },
		parent
	);
}

std::unique_ptr<Polygon2D>&& Renderer::addPolygon2D(const YAML::Node& node, Entity* parent) 
{
	// Load vertices.
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

std::unique_ptr<Circle>&& Renderer::addCircle2D(const YAML::Node& node, Entity* parent) 
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

std::unique_ptr<PolyLine>&& Renderer::addPolyLine(const YAML::Node& node, Entity* parent)
{
	// Load data.
	std::vector<glm::vec2> vertices;
	for (const auto& vertexNode : node["Vertices"])
	{
		YAML::Node vertex = vertexNode.second;
		vertices.push_back({ vertex[0].as<float>(), vertex[1].as<float>() });
	}
	glm::vec4 color = { node["Color"][0].as<float>(), node["Color"][1].as<float>() , node["Color"][2].as<float>() , node["Color"][3].as<float>() };

	if (node["Closed"].as<bool>())
	{
		auto polyline = Renderer::addPolygon2DClear(
				vertices,
				node["Thickness"].as<float>(),
				parent
			);
		polyline->setColor(color); 
		return std::move(polyline);
	}
	else
	{
		auto polyline = Renderer::addPolyLine(
				vertices,
				node["Thickness"].as<float>(),
				color,
				node["Rounded"].as<bool>(),
				parent
			);
		//polyline->setColor(color);
		return std::move(polyline);
	}
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//