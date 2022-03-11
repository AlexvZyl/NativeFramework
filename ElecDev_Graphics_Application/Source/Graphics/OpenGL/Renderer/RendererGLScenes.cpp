//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "OpenGL/Renderer/RendererGL.h"
#include "OpenGL/SceneGL.h"
#include "OpenGL/ShaderGL.h"
#include "Lumen.h"
#include "Graphics/Camera/Camera.h"
#include "Utilities/Profiler/Profiler.h"
#include "Application/Application.h"

//==============================================================================================================================================//
//  General.																																	//
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

void Renderer::initSceneDestruction(Scene* scene)
{
	// Store current scene.
	m_storedScene = m_scene;
	// Bind scene to be destroyed.
	m_scene = scene;
}

void Renderer::doneSceneDestruction()
{
	// Restore scene.
	if (m_scene != m_storedScene) { m_scene = m_storedScene; }
	else						  { m_scene = nullptr;		 }
	// Remove stored scene.
	m_storedScene = nullptr;
}

//==============================================================================================================================================//
//  Rendering.																																	//
//==============================================================================================================================================//

void Renderer::renderScene()
{
	LUMEN_PROFILE_SCOPE("Draw Scene");

	if		(m_scene->m_camera->m_type == CameraType::Standard2D) { render2DScene(m_scene); }
	else if (m_scene->m_camera->m_type == CameraType::Standard3D) { render3DScene(m_scene); }
	else	 return;

	//  Resolve the MSAA.
	m_scene->m_FBO->renderFromMSAA();

	LUMEN_RENDER_PASS();
}

void Renderer::renderScene(Scene* scene)
{
	LUMEN_PROFILE_SCOPE("Draw Scene");

	if		(scene->m_camera->m_type == CameraType::Standard2D)	{ render2DScene(scene); }
	else if (scene->m_camera->m_type == CameraType::Standard3D) { render3DScene(scene); }
	else	 return;

	//  Resolve the MSAA.
	scene->m_FBO->renderFromMSAA();

	LUMEN_RENDER_PASS();
}

void Renderer::render2DScene(Scene* scene)
{
	// ----------- //
	//  S E T U P  //
	// ----------- //

	// Enable blending.
	Renderer::enable(GL_BLEND);
	// Set vewport for the ImGUI context.
	Renderer::setViewport(scene->getViewport());

	// Update camera.
	scene->onUpdate();

	// Render to framebuffer.
	scene->bindFBO();
	scene->clearFBO();

	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// The shader used in rendering.
	Shader* shader = nullptr;

	// Draw background.
	m_shaders["BackgroundShader2D"]->bind();
	Renderer::drawBufferIndexed(scene->m_backgroundVAO.get());

	// Draw basic primitives.
	shader = m_shaders["BasicShader"].get();
	shader->bind();
	shader->setMat4("viewMatrix", scene->getViewMatrix());
	shader->setMat4("projectionMatrix", scene->getProjectionMatrix());
	Renderer::drawBufferIndexed(scene->m_linesVAO.get());
	Renderer::drawBufferIndexed(scene->m_trianglesVAO.get());

	// Draw textured primitives.
	shader = m_shaders["TextureShader"].get();
	shader->bind();
	shader->setMat4("viewMatrix", scene->getViewMatrix());
	shader->setMat4("projectionMatrix", scene->getProjectionMatrix());
	Renderer::loadTextures(scene);
	Renderer::drawBufferIndexed(scene->m_texturedTrianglesVAO.get());

	// Draw Circles.
	shader = m_shaders["CircleShader"].get();
	shader->bind();
	shader->setMat4("viewMatrix", scene->getViewMatrix());
	shader->setMat4("projectionMatrix", scene->getProjectionMatrix());
	Renderer::drawBufferIndexed(scene->m_circlesVAO.get());

	// --------------- //
	//  C L E A N U P  //
	// --------------- //

	// Stop rendering to the current FBO.
	scene->unbindFBO();
	// Disable blending.
	Renderer::disable(GL_BLEND);
}

void Renderer::render3DScene(Scene* scene)
{
	// ----------- //
	//  S E T U P  //
	// ----------- //

	// Enable blending.
	Renderer::enable(GL_BLEND);
	// Set viewport for the ImGUI context.
	Renderer::setViewport(scene->getViewport());

	// Update camera.
	scene->onUpdate();

	// Render to framebuffer.
	scene->bindFBO();
	scene->clearFBO();

	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// The shader used in rendering.
	Shader* shader = nullptr;

	// Draw background.
	m_shaders["BackgroundShader3D"]->bind();
	Renderer::setDepthFunc(GL_EQUAL);
	Renderer::drawBufferIndexed(scene->m_backgroundVAO.get());
	Renderer::setDepthFunc(GL_LESS);

	// Draw basic primitives.
	shader = m_shaders["BasicShader"].get();
	shader->bind();
	shader->setMat4("viewMatrix", scene->getViewMatrix());
	shader->setMat4("projectionMatrix", scene->getProjectionMatrix());
	Renderer::drawBufferIndexed(scene->m_linesVAO.get());
	Renderer::drawBufferIndexed(scene->m_trianglesVAO.get());

	// Draw textured primitives.
	shader = m_shaders["TextureShader"].get();
	shader->bind();
	shader->setMat4("viewMatrix", scene->getViewMatrix());
	shader->setMat4("projectionMatrix", scene->getProjectionMatrix());
	Renderer::loadTextures(scene);
	Renderer::drawBufferIndexed(scene->m_texturedTrianglesVAO.get());

	// Draw Circles.
	shader = m_shaders["CircleShader"].get();
	shader->bind();
	shader->setMat4("viewMatrix", scene->getViewMatrix());
	shader->setMat4("projectionMatrix", scene->getProjectionMatrix());
	Renderer::drawBufferIndexed(scene->m_circlesVAO.get());

	// --------------- //
	//  C L E A N U P  //
	// --------------- //

	// Stop rendering to the current FBO.
	scene->unbindFBO();
	// Disable blending.
	Renderer::disable(GL_BLEND);
}

//==============================================================================================================================================//
//  Default Scenes.																																//
//==============================================================================================================================================//

void Renderer::generateDefaultScenes()
{
	m_default2DScene = std::make_unique<Scene>(CameraType::Standard2D, 900, 900, 1);
	// Render the scene once so that the background can be generated.
	Renderer::render2DScene(m_default2DScene.get());
	m_default2DScene->m_FBO->renderFromMSAA();
}

unsigned Renderer::getDefault2DSceneTexture()
{
	return m_default2DScene->getRenderTexture();
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//