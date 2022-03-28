//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "glad/glad.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "OpenGL/SceneGL.h"
#include "OpenGL/ErrorHandlerGL.h"
#include "OpenGL/Primitives/Polygon.h"
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

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//