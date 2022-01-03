//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Texture.h"
#include "Renderer.h"
#include "Scene.h"
#include "Camera.h"
#include "glad/glad.h"
#include "ErrorHandlerGL.h"
#include "FrameBufferObjectGL.h"
#include "VertexArrayObjectGL.h"
#include "ShaderHandlerGL.h"
#include "Resources/ResourceHandler.h"
#include "Entities/Vertex.h"
#include "Entities/Polygon.h"
#include "Entities/EntityManager.h"
#include "Entities/Circle.h"
#include "Entities/LineSegment.h"
#include "Entities/Text.h"
#include "FontLoader.h"

//==============================================================================================================================================//
//  Static Inisialisation.																														//
//==============================================================================================================================================//

std::map<std::string, std::unique_ptr<Shader>> Renderer::m_shaders;
Scene* Renderer::m_scene = nullptr;
std::unique_ptr<Font> Renderer::m_defaultFont = nullptr;

//==============================================================================================================================================//
//  Utilities.																																	//
//==============================================================================================================================================//

void Renderer::initialise() 
{
	Renderer::compileShaders();
	m_defaultFont = std::make_unique<Font>(msdfLoadFont(ARIAL_NORMAL_JSON));
}

void Renderer::compileShaders()
{
	m_shaders.insert({ "BackgroundShader2D", std::make_unique<Shader>(BACKGROUND_SHADER_2D) });
	m_shaders.insert({ "BackgroundShader3D", std::make_unique<Shader>(BACKGROUND_SHADER_3D) });
	m_shaders.insert({ "BasicShader"       , std::make_unique<Shader>(BASIC_SHADER)			});
	m_shaders.insert({ "TextureShader"     , std::make_unique<Shader>(TEXTURE_SHADER)		});
	m_shaders.insert({ "CircleShader"      , std::make_unique<Shader>(CIRCLE_SHADER)		});
}

//==============================================================================================================================================//
//  Scene.																																		//
//==============================================================================================================================================//

void Renderer::bindScene(Scene* scene) { m_scene = scene; }

void Renderer::unbindScene() { m_scene = nullptr; }

void Renderer::renderScene() 
{
	if		(m_scene->m_camera->m_type == CameraType::Standard2D) { render2DScene(m_scene); }
	else if (m_scene->m_camera->m_type == CameraType::Standard3D) { render3DScene(m_scene); }
}

void Renderer::renderScene(Scene* scene)
{
	if		(scene->m_camera->m_type == CameraType::Standard2D) { render2DScene(scene); }
	else if (scene->m_camera->m_type == CameraType::Standard3D) { render3DScene(scene); }
}
	
//==============================================================================================================================================//
//  Rendering.																																	//
//==============================================================================================================================================//

void Renderer::render2DScene(Scene* scene)
{
	// Remember to update the projection matrices of the shaders.

	// ----------- //
	//  S E T U P  //
	// ----------- //

	// Enable blending.
	GLCall(glEnable(GL_BLEND));
	// Set glViewport for the ImGUI context.
	glm::vec2* viewport = scene->getViewportDimensions();
	GLCall(glViewport(0, 0, (GLsizei)viewport->x, (GLsizei)viewport->y));

	// Update camera.
	scene->updateCamera();

	// Render to frame buffer.
	scene->m_FBO->bind();
	scene->m_FBO->clear();

	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// Draw background.
	m_shaders["BackgroundShader2D"]->bind();
	scene->m_backgroundVAO->render();

	// Draw basic primitives.
	m_shaders["BasicShader"]->bind();
	m_shaders["BasicShader"]->setMat4("viewMatrix", m_scene->getViewMatrix());
	scene->m_linesVAO->render();
	scene->m_trianglesVAO->render();

	// Draw textured primitives.
	m_shaders["TextureShader"]->bind();
	m_shaders["TextureShader"]->setMat4("viewMatrix", m_scene->getViewMatrix());
	scene->m_texturedTrianglesVAO->render();

	// Draw Circles.
	m_shaders["CircleShader"]->bind();
	m_shaders["CircleShader"]->setMat4("viewMatrix", m_scene->getViewMatrix());
	scene->m_circlesVAO->render();

	// --------------- //
	//  C L E A N U P  //
	// --------------- //

	// Stop rendering to the current FBO.
	scene->m_FBO->unbind();
	// Disable blending.
	GLCall(glDisable(GL_BLEND));
}

void Renderer::render3DScene(Scene* scene)
{
	// Remember to update the projection matrices of the shaders.

	// ----------- //
	//  S E T U P  //
	// ----------- //

	// Enable blending.
	GLCall(glEnable(GL_BLEND));
	// Set glViewport for the ImGUI context.
	glm::vec2* viewport = scene->getViewportDimensions();
	GLCall(glViewport(0, 0, (GLsizei)viewport->x, (GLsizei)viewport->y));

	// Update camera.
	scene->updateCamera();

	// Render to frame buffer.
	scene->m_FBO->bind();
	scene->m_FBO->clear();

	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// Draw background.
	m_shaders["BackgroundShader3D"]->bind();
	// Prevent background from clipping the scene.
	GLCall(glDepthFunc(GL_EQUAL));
	scene->m_backgroundVAO->render();
	GLCall(glDepthFunc(GL_LESS));

	// Draw basic primitives.
	m_shaders["BasicShader"]->bind();
	m_shaders["BasicShader"]->setMat4("viewMatrix", m_scene->getViewMatrix());
	scene->m_linesVAO->render();
	scene->m_trianglesVAO->render();

	// Draw textured primitives.
	m_shaders["TextureShader"]->bind();
	m_shaders["TextureShader"]->setMat4("viewMatrix", m_scene->getViewMatrix());
	scene->m_texturedTrianglesVAO->render();

	// Draw Circles.
	m_shaders["CircleShader"]->bind();
	m_shaders["CircleShader"]->setMat4("viewMatrix", m_scene->getViewMatrix());
	scene->m_circlesVAO->render();

	// --------------- //
	//  C L E A N U P  //
	// --------------- //

	// Stop rendering to the current FBO.
	scene->m_FBO->unbind();
	// Disable blending.
	GLCall(glDisable(GL_BLEND));
}

//==============================================================================================================================================//
//  2D Primitives.																																//
//==============================================================================================================================================//

//void Renderer::removePrimitive(unsigned entityID) 
//{
//	m_scene->m_primitives.erase(entityID);
//}

Polygon2D* Renderer::addPolygon2D(std::vector<glm::vec3> vertices, Entity* parent)
{
	m_scene->m_primitives.insert({ EntityManager::getLastID() + 1, std::make_unique<Polygon2D>(vertices, m_scene->m_trianglesVAO.get(), parent) });
	return reinterpret_cast<Polygon2D*>(m_scene->m_primitives[EntityManager::getLastID()].get());
}

//Polygon2DTextured* Renderer::addPolygon2DTextured() 
//{
//
//}

Circle* Renderer::addCircle2D(glm::vec3& center, float radius, glm::vec4& color, float thickness, float fade, Entity* parent) 
{
	m_scene->m_primitives.insert({ EntityManager::getLastID() + 1, std::make_unique<Circle>(m_scene->m_circlesVAO.get(), center, radius, color, thickness, fade, parent)});
	return reinterpret_cast<Circle*>(m_scene->m_primitives[EntityManager::getLastID()].get());
}

Circle* Renderer::addCircle2D(glm::vec2& center, float radius, glm::vec4& color, float thickness, float fade, Entity* parent)
{
	m_scene->m_primitives.insert({ EntityManager::getLastID() + 1, std::make_unique<Circle>(m_scene->m_circlesVAO.get(), center, radius, color, thickness, fade, parent)});
	return reinterpret_cast<Circle*>(m_scene->m_primitives[EntityManager::getLastID()].get());
}

LineSegment* Renderer::addLineSegment2D(glm::vec2 start, glm::vec2 end, Entity* parent, float thickness, glm::vec4 colour)
{
	m_scene->m_primitives.insert({ EntityManager::getLastID() + 1, std::make_unique<LineSegment>(start, end, m_scene->m_trianglesVAO.get(), parent, thickness, colour)});
	return reinterpret_cast<LineSegment*>(m_scene->m_primitives[EntityManager::getLastID()].get());
}

Text* Renderer::addText2D(std::string text, glm::vec3& position, glm::vec4& color, float scale, Entity* parent, std::string horizontalAlignment, std::string verticalAlignment)
{
	m_scene->m_primitives.insert({EntityManager::getLastID() + 1, std::make_unique<Text>(text, position, color, scale,
																					 m_scene->m_texturedTrianglesVAO.get(), *m_defaultFont.get(),
																					 parent, horizontalAlignment, verticalAlignment)});
	return reinterpret_cast<Text*>(m_scene->m_primitives[EntityManager::getLastID()].get());
}

//==============================================================================================================================================//
//  Textures.																																	//
//==============================================================================================================================================//

Texture* Renderer::generateTexture(unsigned resourceID)
{
	m_scene->m_textures.insert({ resourceID, std::make_unique<Texture>(resourceID)});
	return m_scene->m_textures[resourceID].get();
}

Texture* Renderer::getTexture(unsigned resourceID)
{
	// If texture exists, return pointer.
	if (m_scene->m_textures.find(resourceID) != m_scene->m_textures.end())
	{
		return m_scene->m_textures[resourceID].get();
	} // Has to be a better way other than looping through all of the textures.
	// Else, generate new texture and return.
	else 
	{
		return generateTexture(resourceID);
	}
}
 
//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//

