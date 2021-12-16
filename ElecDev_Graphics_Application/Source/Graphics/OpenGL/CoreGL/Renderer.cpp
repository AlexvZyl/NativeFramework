//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Renderer.h"
#include "Scene.h"
#include "Camera.h"
#include "glad/glad.h"
#include "ErrorHandlerGL.h"
#include "FrameBufferObjectGL.h"
#include "VertexArrayObjectGL.h"
#include "ShaderHandlerGL.h"
#include "Resources/ResourceHandler.h"

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
	if (m_scene->m_camera->m_type == CameraType::Standard2D) { render2DScene(m_scene); }
	if (m_scene->m_camera->m_type == CameraType::Standard3D) { render3DScene(m_scene); }
}

void Renderer::renderScene(Scene* scene)
{
	if (scene->m_camera->m_type == CameraType::Standard2D) { render2DScene(scene); }
	if (scene->m_camera->m_type == CameraType::Standard3D) { render3DScene(scene); }
}

//==============================================================================================================================================//
//  Shaders.																																	//
//==============================================================================================================================================//

Shader* Renderer::compileBackgroundShader2D() 
{
	m_backgroundShader2D = std::make_unique<Shader>(BACKGROUND_SHADER_2D);
	return m_backgroundShader2D.get();
}

Shader* Renderer::compileBackgroundShader3D()
{
	m_backgroundShader3D = std::make_unique<Shader>(BACKGROUND_SHADER_3D);
	return m_backgroundShader3D.get();
}

Shader* Renderer::compileBasicShader()
{
	m_basicShader = std::make_unique<Shader>(BASIC_SHADER);
	return m_basicShader.get();
}

Shader* Renderer::compileTextureShader()
{
	m_textureShader = std::make_unique<Shader>(BASIC_SHADER);
	return m_textureShader.get();
}

Shader* Renderer::compileCircleShader()
{
	m_circleShader = std::make_unique<Shader>(BASIC_SHADER);
	return m_circleShader.get();
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
	GLCall(glViewport(0, 0, (GLsizei)scene->m_viewport.x, (GLsizei)scene->m_viewport.y));

	// Update camera.
	scene->updateCamera();

	// Render to frame buffer.
	scene->m_FBO->bind();
	scene->m_FBO->clear();

	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// Draw background.
	m_backgroundShader2D->bind();
	scene->m_backgroundVAO->render();

	// Draw basic entities.
	m_basicShader->bind();
	m_basicShader->setMat4("viewMatrix", m_scene->getViewMatrix());
	scene->m_linesVAO->render();
	scene->m_trianglesVAO->render();

	// Draw textured entities.
	m_textureShader->bind();
	m_textureShader->setMat4("viewMatrix", m_scene->getViewMatrix());
	scene->m_texturedTrianglesVAO->render();

	// Draw Circles.
	m_circleShader->bind();
	m_circleShader->setMat4("viewMatrix", m_scene->getViewMatrix());
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
	// ----------- //
	//  S E T U P  //
	// ----------- //

	// Enable blending.
	GLCall(glEnable(GL_BLEND));
	// Set glViewport for the ImGUI context.
	GLCall(glViewport(0, 0, (GLsizei)scene->m_viewport.x, (GLsizei)scene->m_viewport.y));

	// Update camera.
	scene->updateCamera();

	// Render to frame buffer.
	scene->m_FBO->bind();
	scene->m_FBO->clear();

	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// Draw background.
	m_backgroundShader3D->bind();
	// Prevent background from clipping the scene.
	GLCall(glDepthFunc(GL_EQUAL));
	scene->m_backgroundVAO->render();
	GLCall(glDepthFunc(GL_LESS));

	// Draw basic primitives.
	m_basicShader->bind();
	m_basicShader->setMat4("viewMatrix", m_scene->getViewMatrix());
	scene->m_linesVAO->render();
	scene->m_trianglesVAO->render();

	// Draw textured primitives.
	m_textureShader->bind();
	m_textureShader->setMat4("viewMatrix", m_scene->getViewMatrix());
	scene->m_texturedTrianglesVAO->render();

	// Draw circles.
	m_circleShader->bind();
	m_circleShader->setMat4("viewMatrix", m_scene->getViewMatrix());
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
//  Primitives.																																	//
//==============================================================================================================================================//

Polygon2D* Renderer::addPolygon2D() 
{

}

Polygon2DTextured* Renderer::addPolygon2DTextured() 
{

}

Circle2D* Renderer::addCircle2D() 
{

}

LineSegment2D* Renderer::addLineSegment2D() 
{

}

Text2D* Renderer::addText2D() 
{

}

//==============================================================================================================================================//
//  Primitives.																																	//
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
	}
	// Else generate new texture and return.
	else 
	{
		return generateTexture(resourceID);
	}
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//

