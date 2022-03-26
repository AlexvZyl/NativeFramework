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
#include "Utilities/Logger/Logger.h"
#include "OpenGL/ErrorHandlerGL.h"

//==============================================================================================================================================//
//  General.																																	//
//==============================================================================================================================================//

void Renderer::bindScene(Scene* scene)
{
	s_scene = scene;
}

void Renderer::unbindScene()
{
	s_scene = nullptr;
}

Scene* Renderer::getScene()
{
	return s_scene;
}

void Renderer::initSceneDestruction(Scene* scene)
{
	// Store current scene.
	s_storedScene = s_scene;
	// Bind scene to be destroyed.
	s_scene = scene;
}

void Renderer::doneSceneDestruction()
{
	// Restore scene.
	if (s_scene != s_storedScene) { s_scene = s_storedScene; }
	else						  { s_scene = nullptr;		 }
	// Remove stored scene.
	s_storedScene = nullptr;
}

//==============================================================================================================================================//
//  Rendering.																																	//
//==============================================================================================================================================//

// --------------- //
//  G E N E R A L  //
// --------------- //

void Renderer::renderScene()
{
	Renderer::renderScene(s_scene);
}

void Renderer::renderScene(Scene* scene)
{
	LUMEN_PROFILE_SCOPE("Render Scene");

	// Prepare scene for rendering.
	scene->onRenderInit();

	// Dispatch pipeline.
	switch (scene->m_camera->m_type)
	{
		case CameraType::Standard2D:
			renderingPipeline2D(scene);
			break;

		case CameraType::Standard3D:
			renderingPipeline3D(scene);
			break;

		default:
			LUMEN_LOG_ERROR("Scene has unknown camera type.", "Renderer");
			return;
	}

	// Done with rendering.
	scene->onRenderCleanup();
}

void Renderer::backgroundPass(Scene* scene) 
{
	LUMEN_RENDER_PASS();

	// Setup.
	Renderer::enable(GL_DEPTH_TEST);
	
	// Draw background.
	s_shaders["BackgroundShader"]->bind();
	Renderer::setDepthFunc(GL_ALWAYS);
	Renderer::drawBufferIndexed(scene->m_backgroundVAO.get());
	Renderer::setDepthFunc(GL_LESS);
}

// ----------------------- //
//  2 D   P I P E L I N E  //
// ----------------------- //

void Renderer::renderingPipeline2D(Scene* scene) 
{
	if (Renderer::s_pipelineControls["Background"])
	{
		Renderer::backgroundPass(scene);
	}

	if (Renderer::s_pipelineControls["Geometry"])
	{
		Renderer::geometryPass2D(scene);
	}

	if (Renderer::s_pipelineControls["Outline"])
	{
		Renderer::objectOutliningPass2D(scene);
	}
}

void Renderer::geometryPass2D(Scene* scene)
{
	LUMEN_RENDER_PASS();

	// ----------- //
	//  S E T U P  //
	// ----------- //

	Renderer::enable(GL_DEPTH_TEST);
	Renderer::enable(GL_BLEND);

	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// The shader used in rendering.
	Shader* shader = nullptr;

	// Draw basic primitives.
	shader = s_shaders["BasicShader"].get();
	shader->bind();
	shader->setMat4("viewMatrix", scene->getViewMatrix());
	shader->setMat4("projectionMatrix", scene->getProjectionMatrix());
	Renderer::drawBufferIndexed(scene->m_linesVAO.get());
	Renderer::drawBufferIndexed(scene->m_trianglesVAO.get());

	// Draw textured primitives.
	shader = s_shaders["TextureShader"].get();
	shader->bind();
	shader->setMat4("viewMatrix", scene->getViewMatrix());
	shader->setMat4("projectionMatrix", scene->getProjectionMatrix());
	Renderer::loadTextures(scene);
	Renderer::drawBufferIndexed(scene->m_texturedTrianglesVAO.get());

	// Draw Circles.
	shader = s_shaders["CircleShader"].get();
	shader->bind();
	shader->setMat4("viewMatrix", scene->getViewMatrix());
	shader->setMat4("projectionMatrix", scene->getProjectionMatrix());
	Renderer::drawBufferIndexed(scene->m_circlesVAO.get());
}

void Renderer::objectOutliningPass2D(Scene* scene) 
{
	LUMEN_RENDER_PASS();

	// ----------- //
	//  S E T U P  //
	// ----------- //

	Renderer::enable(GL_DEPTH_TEST);
	Renderer::enable(GL_BLEND);
	GLCall(glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// The shader used in rendering.
	Shader* shader = nullptr;

	if (Renderer::s_pipelineControls["Background"])
	{
		// Draw background.
		s_shaders["OutlineBackgroundShader"]->bind();
		Renderer::setDepthFunc(GL_ALWAYS);
		Renderer::drawBufferIndexed(scene->m_backgroundVAO.get());
		Renderer::setDepthFunc(GL_LESS);
	}

	// Draw basic primitives.
	shader = s_shaders["OutlineShader"].get();
	shader->bind();
	shader->setMat4("viewMatrix", scene->getViewMatrix());
	shader->setMat4("projectionMatrix", scene->getProjectionMatrix());
	Renderer::drawBufferIndexed(scene->m_linesVAO.get());
	Renderer::drawBufferIndexed(scene->m_trianglesVAO.get());

	// Draw textured primitives.
	shader = s_shaders["OutlineShaderTextures"].get();
	shader->bind();
	shader->setMat4("viewMatrix", scene->getViewMatrix());
	shader->setMat4("projectionMatrix", scene->getProjectionMatrix());
	Renderer::loadTextures(scene);
	Renderer::drawBufferIndexed(scene->m_texturedTrianglesVAO.get());

	// Draw Circles.
	shader = s_shaders["OutlineShaderCircle"].get();
	shader->bind();
	shader->setMat4("viewMatrix", scene->getViewMatrix());
	shader->setMat4("projectionMatrix", scene->getProjectionMatrix());
	Renderer::drawBufferIndexed(scene->m_circlesVAO.get());

	// Render outline with post processing.
	Renderer::setDepthFunc(GL_ALWAYS);
	if (Renderer::s_pipelineControls["OutlinePostProc"]) 
	{
		shader = s_shaders["OutlinePostProc"].get();
		shader->bind();
		shader->setFloat("width", scene->getViewport()[2]);
		shader->setFloat("height", scene->getViewport()[3]);
		Renderer::drawTextureOverFBOAttachment(scene->m_FBO->m_outlineColorTextureID, GL_COLOR_ATTACHMENT0, shader);
	}
	// Render outline texture directly.
	else
	{
		Renderer::drawTextureOverFBOAttachment(scene->m_FBO->m_outlineColorTextureID, GL_COLOR_ATTACHMENT0, s_shaders["StaticTextureShader"].get());
	}
	Renderer::setDepthFunc(GL_LESS);
}

// ----------------------- //
//  3 D   P I P E L I N E  //
// ----------------------- //

void Renderer::renderingPipeline3D(Scene* scene)
{
	Renderer::geometryPass3D(scene);
}

void Renderer::geometryPass3D(Scene* scene)
{
	LUMEN_RENDER_PASS();

	// ----------- //
	//  S E T U P  //
	// ----------- //

	Renderer::enable(GL_BLEND);
	Renderer::setViewport(scene->getViewport());
	
	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// The shader used in rendering.
	Shader* shader = nullptr;

	// Draw basic primitives.
	shader = s_shaders["BasicShader"].get();
	shader->bind();
	shader->setMat4("viewMatrix", scene->getViewMatrix());
	shader->setMat4("projectionMatrix", scene->getProjectionMatrix());
	Renderer::drawBufferIndexed(scene->m_linesVAO.get());
	Renderer::drawBufferIndexed(scene->m_trianglesVAO.get());

	// Draw textured primitives.
	shader = s_shaders["TextureShader"].get();
	shader->bind();
	shader->setMat4("viewMatrix", scene->getViewMatrix());
	shader->setMat4("projectionMatrix", scene->getProjectionMatrix());
	Renderer::loadTextures(scene);
	Renderer::drawBufferIndexed(scene->m_texturedTrianglesVAO.get());

	// Draw Circles.
	shader = s_shaders["CircleShader"].get();
	shader->bind();
	shader->setMat4("viewMatrix", scene->getViewMatrix());
	shader->setMat4("projectionMatrix", scene->getProjectionMatrix());
	Renderer::drawBufferIndexed(scene->m_circlesVAO.get());
}

//==============================================================================================================================================//
//  Default Scenes.																																//
//==============================================================================================================================================//

void Renderer::generateDefaultScenes()
{
	m_default2DScene = std::make_unique<Scene>(CameraType::Standard2D, 900, 900);
	// Render the scene once so that the background can be generated.
	Renderer::renderScene(m_default2DScene.get());
}

unsigned Renderer::getDefault2DSceneTexture()
{
	return m_default2DScene->getRenderTexture();
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//