//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "glad/glad.h"
#include "Resources/ResourceHandler.h"
#include "OpenGL/ShaderGL.h"
#include "OpenGL/RendererGL.h"
#include "OpenGL/CameraGL.h"
#include "OpenGL/SceneGL.h"
#include "OpenGL/FontLoaderGL.h"
#include "OpenGL/ErrorHandlerGL.h"
#include "OpenGL/Primitives/Polygon.h"
#include "OpenGL/Primitives/TextureGL.h"
#include "OpenGL/Primitives/Circle.h"
#include "OpenGL/Primitives/LineSegment.h"
#include "OpenGL/Primitives/Text.h"
#include "OpenGL/Primitives/Primitive.h"
#include "OpenGL/Primitives/Vertex.h"
#include "OpenGL/Entities/EntityManager.h"
#include "OpenGL/Buffers/VertexArrayObjectGL.h"
#include "OpenGL/Buffers/FrameBufferObjectGL.h"

//==============================================================================================================================================//
//  Static Inisialisation.																														//
//==============================================================================================================================================//

std::map<std::string, std::unique_ptr<Shader>> Renderer::m_shaders;
Scene* Renderer::m_scene = nullptr;
std::unique_ptr<Font> Renderer::m_defaultFont = nullptr;

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

Scene* Renderer::getScene()
{
	return m_scene;
}

void Renderer::renderScene() 
{
	if	    (m_scene->m_camera->m_type == CameraType::Standard2D) { render2DScene(m_scene); }
	else if (m_scene->m_camera->m_type == CameraType::Standard3D) { render3DScene(m_scene); }

	//  Resolve the MSAA.
	m_scene->m_FBO->renderFromMSAA();
}

void Renderer::renderScene(Scene* scene)
{
	if	    (scene->m_camera->m_type == CameraType::Standard2D) { render2DScene(scene); }
	else if (scene->m_camera->m_type == CameraType::Standard3D) { render3DScene(scene); }

	//  Resolve the MSAA.
	scene->m_FBO->renderFromMSAA();
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
	enable(GL_BLEND);
	// Set vewport for the ImGUI context.
	setViewport(scene->getViewport());

	// Update camera.
	scene->updateCamera();

	// Render to framebffer.
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
	loadTextures(scene);
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
	disable(GL_BLEND);
}

void Renderer::render3DScene(Scene* scene)
{
	// ----------- //
	//  S E T U P  //
	// ----------- //

	// Enable blending.
	enable(GL_BLEND);
	// Set viewport for the ImGUI context.
	setViewport(scene->getViewport());

	// Update camera.
	scene->updateCamera();

	// Render to framebuffer.
	scene->bindFBO();
	scene->clearFBO();

	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// Draw background.
	m_shaders["BackgroundShader3D"]->bind();
	setDepthFunc(GL_EQUAL);
	scene->m_backgroundVAO->render();
	setDepthFunc(GL_LESS);

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
	loadTextures(scene);
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
	disable(GL_BLEND);
}

//==============================================================================================================================================//
//  Textures.																																	//
//==============================================================================================================================================//

Texture* Renderer::generateTexture(unsigned resourceID, const std::string& name)
{
	m_scene->m_textures.insert({ name, std::make_unique<Texture>(resourceID, name) });
	return m_scene->m_textures[name].get();
}

void Renderer::addTexture(const Texture& texture) 
{
	m_scene->m_textures.insert({texture.m_name, std::make_unique<Texture>(texture)});
}

Texture* Renderer::getTexture(const std::string& name)
{
	return m_scene->m_textures[name].get();
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
	int count = 2;
	for (auto& [name, texture] : m_scene->m_textures)
	{
		GLCall(glBindTextureUnit(count, texture->m_rendererID));
		count++;
	}
}

//==============================================================================================================================================//
//  Removing 2D Primitives.																														//
//==============================================================================================================================================//

void Renderer::remove(Primitive<VertexData>* primitive)
{
	m_scene->m_primitives.erase(primitive->m_entityID);
}

void Renderer::remove(Primitive<VertexDataTextured>* primitive)
{
	m_scene->m_primitives.erase(primitive->m_entityID);
}

void Renderer::remove(Primitive<VertexDataCircle>* primitive)
{
	m_scene->m_primitives.erase(primitive->m_entityID);
}

//==============================================================================================================================================//
//  Adding 2D Primitives.																														//
//==============================================================================================================================================//

Polygon2D* Renderer::addPolygon2D(const std::vector<glm::vec3>& vertices, Entity* parent)
{
	unsigned id = EntityManager::peakNextID();
	m_scene->m_primitives.insert({ id, std::make_unique<Polygon2D>(vertices, m_scene->m_trianglesVAO.get(), parent)});
	return dynamic_cast<Polygon2D*>(m_scene->m_primitives[id].get());
}

Polygon2D* Renderer::addPolygon2DClear(const std::vector<glm::vec3>& vertices, Entity* parent)
{
	unsigned id = EntityManager::peakNextID();
	m_scene->m_primitives.insert({ id, std::make_unique<Polygon2D>(vertices, m_scene->m_linesVAO.get(), parent) });
	return dynamic_cast<Polygon2D*>(m_scene->m_primitives[id].get());
}

Circle* Renderer::addCircle2D(const glm::vec3& center, float radius, const glm::vec4& color, float thickness, float fade, Entity* parent) 
{
	unsigned id = EntityManager::peakNextID();
	m_scene->m_primitives.insert({ id, std::make_unique<Circle>(m_scene->m_circlesVAO.get(), center, radius, color, thickness, fade, parent)});
	return dynamic_cast<Circle*>(m_scene->m_primitives[id].get());
}

Circle* Renderer::addCircle2D(const glm::vec2& center, float radius, const glm::vec4& color, float thickness, float fade, Entity* parent)
{
	unsigned id = EntityManager::peakNextID();
	m_scene->m_primitives.insert({ id, std::make_unique<Circle>(m_scene->m_circlesVAO.get(), center, radius, color, thickness, fade, parent) });
	return dynamic_cast<Circle*>(m_scene->m_primitives[id].get());
}

LineSegment* Renderer::addLineSegment2D(const glm::vec2& start, const glm::vec2& end, float thickness, const glm::vec4& colour, Entity* parent)
{
	unsigned id = EntityManager::peakNextID();
	m_scene->m_primitives.insert({ id, std::make_unique<LineSegment>(start, end, m_scene->m_trianglesVAO.get(), parent, thickness, colour) });
	return dynamic_cast<LineSegment*>(m_scene->m_primitives[id].get());
}

Text* Renderer::addText2D(const std::string& text, const glm::vec3& position, const glm::vec4& color, float scale, const std::string& horizontalAlignment, const std::string& verticalAlignment, Entity* parent)
{
	unsigned id = EntityManager::peakNextID();
	m_scene->m_primitives.insert({ id, std::make_unique<Text>(text, position, color, scale,
				       										  m_scene->m_texturedTrianglesVAO.get(), m_defaultFont.get(),
				       										  parent, horizontalAlignment, verticalAlignment)});
	
	return dynamic_cast<Text*>(m_scene->m_primitives[id].get());
}

//==============================================================================================================================================//
//  Setup.																																		//
//==============================================================================================================================================//

void Renderer::initialise()
{
	// OpenGL settings.
	GLCall(glEnable(GL_MULTISAMPLE));                           // Enables MSAA.
	GLCall(glEnable(GL_DEPTH_TEST));                            // Enables depth testing for the OpenGL scenes.
	GLCall(glDepthFunc(GL_LESS));                               // Set the function used with depth testing.
	GLCall(glEnable(GL_BLEND));                                 // Enable blending for alpha channels.
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));  // Set blend function.  This is the standard setting.
	GLCall(glClearColor(0.08f, 0.08f, 0.10f, 1.00f));           // Set the color to which OpenGL clears.

	// Compiles the shaders used by the renderer and the FBO.
	Renderer::compileShaders();
	// Loads the default font to be used by the renderer.
	Renderer::loadDefaultFont();
}

void Renderer::loadDefaultFont()
{
	m_defaultFont = std::make_unique<Font>(msdfLoadFont(ARIAL_NORMAL_JSON, ARIAL_NORMAL_PNG));
}

void Renderer::compileShaders()
{
	// Renderer shaders.
	m_shaders.insert({ "BackgroundShader2D" , std::make_unique<Shader>(BACKGROUND_SHADER_2D) });
	m_shaders.insert({ "BackgroundShader3D" , std::make_unique<Shader>(BACKGROUND_SHADER_3D) });
	m_shaders.insert({ "BasicShader"        , std::make_unique<Shader>(BASIC_SHADER) });
	m_shaders.insert({ "TextureShader"      , std::make_unique<Shader>(TEXTURE_SHADER) });
	m_shaders.insert({ "CircleShader"       , std::make_unique<Shader>(CIRCLE_SHADER) });
	// FBO shader.
	FrameBufferObject::m_shader = std::make_unique<Shader>(STATIC_TEXTURE_SHADER);
}

//==============================================================================================================================================//
//  Utilities.																																	//
//==============================================================================================================================================//

void Renderer::clear() 
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::finish() 
{
	GLCall(glFinish());
}

void Renderer::enable(unsigned attribute)
{
	GLCall(glEnable(attribute));
}

void Renderer::disable(unsigned attribute)
{
	GLCall(glDisable(attribute));
}

void Renderer::setDepthFunc(unsigned function) 
{
	GLCall(glDepthFunc(function));
}

void Renderer::setViewport(const glm::vec2& viewport)
{
	GLCall(glViewport(0, 0, (int)viewport[0], (int)viewport[1]));
}

void Renderer::setViewport(const glm::vec4& viewport)
{
	GLCall(glViewport((int)viewport[0], (int)viewport[1], (int)viewport[2], (int)viewport[3]));
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
