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
#include "CoreGL/Entities/Primitive.h"
#include "CoreGL/Entities/Vertex.h"
#include "CoreGL/ErrorHandlerGL.h"
#include "CoreGL/VertexArrayObjectGL.h"

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
	Renderer::loadDefaultFont();
}

void Renderer::loadDefaultFont() 
{
	m_defaultFont = std::make_unique<Font>(msdfLoadFont(ARIAL_NORMAL_JSON, ARIAL_NORMAL_PNG));
}

void Renderer::compileShaders()
{
	m_shaders.insert({ "BackgroundShader2D" , std::make_unique<Shader>(BACKGROUND_SHADER_2D) });
	m_shaders.insert({ "BackgroundShader3D" , std::make_unique<Shader>(BACKGROUND_SHADER_3D) });
	m_shaders.insert({ "BasicShader"        , std::make_unique<Shader>(BASIC_SHADER)		 });
	m_shaders.insert({ "TextureShader"      , std::make_unique<Shader>(TEXTURE_SHADER)		 });
	m_shaders.insert({ "CircleShader"       , std::make_unique<Shader>(CIRCLE_SHADER)		 });
}

Scene* Renderer::getScene() 
{
	return Renderer::m_scene;
}

//==============================================================================================================================================//
//  Scene.																																		//
//==============================================================================================================================================//

void Renderer::bindScene(Scene* scene) 
{ 
	m_scene = scene; 
}

void Renderer::unbindScene() 
{ 
	m_scene = nullptr; 
}

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
	// ----------- //
	//  S E T U P  //
	// ----------- //

	// Enable blending.
	GLCall(glEnable(GL_BLEND));
	// Set glViewport for the ImGUI context.
	glm::vec2 viewport = scene->getViewport();
	GLCall(glViewport(0, 0, (GLsizei)viewport.x, (GLsizei)viewport.y));

	// Update camera.
	scene->updateCamera();

	// Render to frame buffer.
	scene->bindFBO();
	scene->clearFBO();

	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// Draw background.
	m_shaders["BackgroundShader2D"]->bind();
	scene->m_backgroundVAO->render();

	// Draw basic primitives.
	m_shaders["BasicShader"]->bind();
	m_shaders["BasicShader"]->setMat4("viewMatrix", scene->getViewMatrix());
	m_shaders["BasicShader"]->setMat4("projectionMatrix", scene->getProjectionMatrix());
	scene->m_linesVAO->render();
	scene->m_trianglesVAO->render();

	// Draw textured primitives.
	m_shaders["TextureShader"]->bind();
	m_shaders["TextureShader"]->setMat4("viewMatrix", scene->getViewMatrix());
	m_shaders["TextureShader"]->setMat4("projectionMatrix", scene->getProjectionMatrix());
	Renderer::loadTextures(scene);
	scene->m_texturedTrianglesVAO->render();

	// Draw Circles.
	m_shaders["CircleShader"]->bind();
	m_shaders["CircleShader"]->setMat4("viewMatrix", scene->getViewMatrix());
	m_shaders["CircleShader"]->setMat4("projectionMatrix", scene->getProjectionMatrix());
	scene->m_circlesVAO->render();

	// --------------- //
	//  C L E A N U P  //
	// --------------- //

	// Stop rendering to the current FBO.
	scene->unbindFBO();
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
	glm::vec2 viewport = scene->getViewport();
	GLCall(glViewport(0, 0, (GLsizei)viewport.x, (GLsizei)viewport.y));

	// Update camera.
	scene->updateCamera();

	// Render to frame buffer.
	scene->bindFBO();
	scene->clearFBO();

	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// Draw background.
	m_shaders["BackgroundShader3D"]->bind();
	GLCall(glDepthFunc(GL_EQUAL));
	scene->m_backgroundVAO->render();
	GLCall(glDepthFunc(GL_LESS));

	// Draw basic primitives.
	m_shaders["BasicShader"]->bind();
	m_shaders["BasicShader"]->setMat4("viewMatrix", scene->getViewMatrix());
	m_shaders["BasicShader"]->setMat4("projectionMatrix", scene->getProjectionMatrix());
	scene->m_linesVAO->render();
	scene->m_trianglesVAO->render();

	// Draw textured primitives.
	m_shaders["TextureShader"]->bind();
	m_shaders["TextureShader"]->setMat4("viewMatrix", scene->getViewMatrix());
	m_shaders["TextureShader"]->setMat4("projectionMatrix", scene->getProjectionMatrix());
	Renderer::loadTextures(scene);
	scene->m_texturedTrianglesVAO->render();

	// Draw Circles.
	m_shaders["CircleShader"]->bind();
	m_shaders["CircleShader"]->setMat4("viewMatrix", scene->getViewMatrix());
	m_shaders["CircleShader"]->setMat4("projectionMatrix", scene->getProjectionMatrix());
	scene->m_circlesVAO->render();

	// --------------- //
	//  C L E A N U P  //
	// --------------- //

	// Stop rendering to the current FBO.
	scene->unbindFBO();
	// Disable blending.
	GLCall(glDisable(GL_BLEND));
}
//==============================================================================================================================================//
//  Textures.																																	//
//==============================================================================================================================================//

Texture* Renderer::generateTexture(unsigned resourceID)
{
	m_scene->m_textures.insert({ resourceID, std::make_unique<Texture>(resourceID) });
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

void Renderer::loadTextures(Scene* scene)
{	// SHADER MUST BE BOUND.

	// Create sampler variable.
	int textureCount = scene->m_textures.size() + 2;
	std::vector<int> samplers = { 0 };
	for (int i = 1; i < textureCount; i++) { samplers.push_back(i); }
	// Prepare shader.
	GLCall(auto loc = glGetUniformLocation(m_shaders["TextureShader"]->m_rendererID, "f_textures"));
	GLCall(glUniform1iv(loc, textureCount, samplers.data()));
	// Load font.
	GLCall(glBindTextureUnit(1, m_defaultFont->textureID));
	// Load all of the other textures.
	for (int i = 2; i < textureCount; i++) { GLCall(glBindTextureUnit(i, scene->m_textures[i]->m_GLID)); }
}

//==============================================================================================================================================//
//  Removing 2D Primitives.																														//
//==============================================================================================================================================//

void Renderer::remove(Primitive<VertexData>* primitive)
{
	primitive->wipeGPU();
	m_scene->m_primitives.erase(primitive->m_entityID);
}

void Renderer::remove(Primitive<VertexDataTextured>* primitive)
{
	primitive->wipeGPU();
	m_scene->m_primitives.erase(primitive->m_entityID);
}

void Renderer::remove(Primitive<VertexDataCircle>* primitive)
{
	primitive->wipeGPU();
	m_scene->m_primitives.erase(primitive->m_entityID);
}

//==============================================================================================================================================//
//  Adding 2D Primitives.																														//
//==============================================================================================================================================//

Polygon2D* Renderer::addPolygon2D(std::vector<glm::vec3> vertices, Entity* parent)
{
	m_scene->m_primitives.insert({ EntityManager::getLastID() + 1, std::make_unique<Polygon2D>(vertices, m_scene->m_trianglesVAO.get(), parent) });
	return reinterpret_cast<Polygon2D*>(m_scene->m_primitives[EntityManager::getLastID()].get());
}

Polygon2D* Renderer::addPolygon2DClear(std::vector<glm::vec3> vertices, Entity* parent)
{
	m_scene->m_primitives.insert({ EntityManager::getLastID() + 1, std::make_unique<Polygon2D>(vertices, m_scene->m_linesVAO.get(), parent) });
	return reinterpret_cast<Polygon2D*>(m_scene->m_primitives[EntityManager::getLastID()].get());
}

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

LineSegment* Renderer::addLineSegment2D(glm::vec2 start, glm::vec2 end, float thickness, glm::vec4 colour, Entity* parent)
{
	m_scene->m_primitives.insert({ EntityManager::getLastID() + 1, std::make_unique<LineSegment>(start, end, m_scene->m_trianglesVAO.get(), parent, thickness, colour)});
	return reinterpret_cast<LineSegment*>(m_scene->m_primitives[EntityManager::getLastID()].get());
}

Text* Renderer::addText2D(std::string text, glm::vec3& position, glm::vec4& color, float scale, std::string horizontalAlignment, std::string verticalAlignment, Entity* parent)
{
	m_scene->m_primitives.insert({EntityManager::getLastID() + 1, std::make_unique<Text>(text, position, color, scale,
																						 m_scene->m_texturedTrianglesVAO.get(), *m_defaultFont.get(),
																						 parent, horizontalAlignment, verticalAlignment)});
	return reinterpret_cast<Text*>(m_scene->m_primitives[EntityManager::getLastID()].get());
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//