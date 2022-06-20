//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "OpenGL/Renderer/RendererGL.h"
#include "OpenGL/SceneGL.h"
#include "OpenGL/ShaderGL.h"
#include "OpenGL/ErrorHandlerGL.h"
#include "OpenGL/Primitives/Grid.h"
#include "Lumen/Lumen.h"
#include "Graphics/Camera/Camera.h"
#include "Utilities/Profiler/Profiler.h"
#include "Application/Application.h"
#include "Utilities/Logger/Logger.h"

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
	storeAndBindScene(scene);
}

void Renderer::doneSceneDestruction()
{
	// Restore scene.
	if (s_scene != s_storedScenes.back()) 
	{ 
		s_scene = s_storedScenes.back();
	}
	else									
	{ 
		s_scene = nullptr;		 
	}
	// Remove stored scene.
	s_storedScenes.pop_back();
}

void Renderer::storeAndBindScene(Scene* scene) 
{
	s_storedScenes.push_back(s_scene);
	Renderer::bindScene(scene);	
}

void Renderer::restoreAndUnbindScene() 
{
	Renderer::bindScene(s_storedScenes.back());
	s_storedScenes.pop_back();
}

//==============================================================================================================================================//
//  Rendering.																																	//
//==============================================================================================================================================//

// --------------- //
//  G E N E R A L  //
// --------------- //

void Renderer::renderScene()
{
	if (s_scene) Renderer::renderScene(s_scene);
	else LUMEN_LOG_WARN("No scene bound.", "Renderer");
}

void Renderer::renderScene(Scene* scene)
{
	LUMEN_PROFILE_SCOPE("Render Scene");

	// Update camera data.
	scene->getCamera().onUpdate();

	// Dispatch pipeline.
	switch (scene->getCamera().getType())
	{
		case CameraType::Standard2D:
			if (Renderer::MSAA == FrameBufferSamples::NORMAL) 
				 renderingPipeline2D(scene);
			else renderingPipeline2DMSAA(scene);
			break;

		case CameraType::Standard3D:
			renderingPipeline3D(scene);
			break;

		default:
			LUMEN_LOG_ERROR("Scene has unknown camera type.", "Renderer");
			return;
	}
}

void Renderer::renderingPipeline2D(Scene* scene) 
{
	// Setup.
	Renderer::enable(GL_DEPTH_TEST);
	Renderer::enable(GL_BLEND);
	Renderer::setDepthFunc(GL_LESS);

	// FBO.
	scene->m_renderFBO.bind();
	scene->m_renderFBO.bindDrawBuffers();
	Renderer::clearWithColor(Renderer::backgroundColor);
	Renderer::clearDepthStencil();

	if (Renderer::s_pipelineControls["Grid"] && scene->m_grid->isEnabled())
		Renderer::gridPass(scene);

	if (Renderer::s_pipelineControls["Geometry"])
		Renderer::geometryPass2D(scene);

	if (Renderer::s_pipelineControls["Outline"])
		Renderer::objectOutliningPass2D(scene);

	// Unbind target.
	scene->m_renderFBO.unbind();
}

void Renderer::renderingPipeline2DMSAA(Scene* scene)
{
	// Setup.
	Renderer::enable(GL_DEPTH_TEST);
	Renderer::enable(GL_BLEND);
	Renderer::setDepthFunc(GL_LESS);

	// MSAA FBO.
	scene->m_msaaFBO.bind();
	Renderer::clearWithColor(Renderer::backgroundColor);
	Renderer::clearDepthStencil();

	// MSAA pass.
	scene->m_msaaFBO.bindDrawBuffers();
	if (Renderer::s_pipelineControls["Geometry"])
		Renderer::geometryPass2D(scene);

	// Non MSAA FBO.
	scene->m_renderFBO.bind();
	Renderer::clearWithColor(Renderer::backgroundColor);
	Renderer::clearDepthStencil();

	// Resolve MSAA.
	Renderer::resolveMSAA(scene->m_msaaFBO, FrameBufferAttachmentSlot::COLOR_0, scene->m_renderFBO, FrameBufferAttachmentSlot::COLOR_0);
	Renderer::blit(scene->m_msaaFBO, FrameBufferAttachmentSlot::COLOR_1, scene->m_renderFBO, FrameBufferAttachmentSlot::COLOR_1);
	Renderer::blitDepthStencil(scene->m_msaaFBO, scene->m_renderFBO);

	// Non MSAA pass.
	scene->m_renderFBO.bindDrawBuffers();
	if (Renderer::s_pipelineControls["Grid"] && scene->m_grid->isEnabled())
		Renderer::gridPass(scene);
	
	// Render outline.
	if (Renderer::s_pipelineControls["Outline"])
	{
		Renderer::blit(scene->m_msaaFBO, FrameBufferAttachmentSlot::COLOR_2, scene->m_renderFBO, FrameBufferAttachmentSlot::COLOR_2, GL_LINEAR);
		Renderer::objectOutliningPass2D(scene);
	}

	// Unbind target.
	scene->m_renderFBO.unbind();
}

void Renderer::backgroundPass(Scene* scene)
{
	s_shaders["BackgroundShader"]->bind();
	Renderer::drawBufferIndexed(scene->m_backgroundBuffer);
}

void Renderer::gridPass(Scene* scene) 
{
	// Setup shader.
	Shader* shader = s_shaders["BasicShader"].get();
	Grid& grid = scene->getGrid();
	Camera& camera = scene->getCamera();
	shader->bind();
	shader->setMat4("viewProjMatrix", grid.getViewProjectionMatrix(camera));

	// Draw render.
	Renderer::drawBufferIndexed(grid.m_originBuffer);
	Renderer::drawBufferIndexed(grid.m_coarseBuffer);
	Renderer::drawBufferIndexed(grid.m_fineBuffer);
}

void Renderer::geometryPass2D(Scene* scene)
{
	LUMEN_RENDER_PASS();

	// The shader used in rendering.
	Shader* shader = nullptr;
	Camera& camera = scene->getCamera();

	// Draw basic primitives.
	shader = s_shaders["BasicShader"].get();
	shader->bind();
	shader->setMat4("viewProjMatrix", camera.getViewProjectionMatrix());
	Renderer::drawBufferIndexed(scene->m_linesBuffer);
	Renderer::drawBufferIndexed(scene->m_trianglesBuffer);

	// Draw Circles.
	shader = s_shaders["CircleShader"].get();
	shader->bind();
	shader->setMat4("viewProjMatrix", camera.getViewProjectionMatrix());
	Renderer::drawBufferIndexed(scene->m_circlesBuffer);

	// Draw textured primitives.
	shader = s_shaders["TextureShader"].get();
	shader->bind();
	shader->setMat4("viewProjMatrix", camera.getViewProjectionMatrix());
	Renderer::loadTextures(scene);
	Renderer::drawBufferIndexed(scene->m_texturedTrianglesBuffer);
}

void Renderer::objectOutliningPass2D(Scene* scene) 
{
	LUMEN_RENDER_PASS();

	// Render outline with post processing.
	if (Renderer::s_pipelineControls["OutlinePostProc"]) 
	{
		Shader* shader = nullptr;
		Camera& camera = scene->getCamera();
		shader = s_shaders["OutlinePostProc"].get();
		shader->bind();
		shader->setFloat("width",  camera.getViewportSize().x);
		shader->setFloat("height", camera.getViewportSize().y);
		Renderer::drawTextureOverFBOAttachment(scene->m_renderFBO, FrameBufferAttachmentSlot::COLOR_0, scene->m_renderFBO.getAttachment(FrameBufferAttachmentSlot::COLOR_2).rendererID, shader);

	}
	// Render outline texture directly.
	else
	{
		Renderer::drawTextureOverFBOAttachment(scene->m_renderFBO, FrameBufferAttachmentSlot::COLOR_0, scene->m_renderFBO.getAttachment(FrameBufferAttachmentSlot::COLOR_2).rendererID, s_shaders["StaticTextureShader"].get());
	}
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
	Renderer::setViewport(scene->getCamera().getViewport());
	
	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// The shader used in rendering.
	Shader* shader = nullptr;
	Camera& camera = scene->getCamera();

	// Draw basic primitives.
	shader = s_shaders["BasicShader"].get();
	shader->bind();
	shader->setMat4("viewProjMatrix", camera.getViewProjectionMatrix());
	Renderer::drawBufferIndexed(scene->m_linesBuffer);
	Renderer::drawBufferIndexed(scene->m_trianglesBuffer);

	// Draw textured primitives.
	shader = s_shaders["TextureShader"].get();
	shader->bind();
	shader->setMat4("viewProjMatrix", camera.getViewProjectionMatrix());
	Renderer::loadTextures(scene);
	Renderer::drawBufferIndexed(scene->m_texturedTrianglesBuffer);

	// Draw Circles.
	shader = s_shaders["CircleShader"].get();
	shader->bind();
	shader->setMat4("viewProjMatrix", camera.getViewProjectionMatrix());
	Renderer::drawBufferIndexed(scene->m_circlesBuffer);
}

//==============================================================================================================================================//
//  Default Scenes.																																//
//==============================================================================================================================================//

void Renderer::generateDefaultScenes()
{
	m_default2DScene = std::make_unique<Scene>(CameraType::Standard2D, glm::vec2(500,500));
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