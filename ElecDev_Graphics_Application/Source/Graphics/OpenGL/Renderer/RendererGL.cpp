//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "glad/glad.h"
#include "Resources/ResourceHandler.h"
#include "OpenGL/ShaderGL.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "OpenGL/ErrorHandlerGL.h"
#include "OpenGL/Buffers/FrameBufferObjectGL.h"
#include "Graphics/Fonts/FontLoader.h"
#include "Application/Application.h"

//==============================================================================================================================================//
//  Static Inisialisation.																														//
//==============================================================================================================================================//

std::map<std::string, std::unique_ptr<Shader>> Renderer::m_shaders;
Scene* Renderer::m_scene = nullptr;
Scene* Renderer::m_storedScene = nullptr;
std::unique_ptr<Font> Renderer::m_defaultFont = nullptr;
std::unique_ptr<Scene> Renderer::m_default2DScene = nullptr;

//==============================================================================================================================================//
//  Setup.																																		//
//==============================================================================================================================================//

void Renderer::shutdown()
{
	m_shaders.clear();
	m_default2DScene.reset();
	m_defaultFont.reset();
	FrameBufferObject::m_shader.reset();
}

void Renderer::initialise()
{
	// OpenGL settings.
	GLCall(glEnable(GL_MULTISAMPLE));                           // Enables MSAA.
	GLCall(glEnable(GL_DEPTH_TEST));                            // Enables depth testing for the OpenGL scenes.
	GLCall(glDepthFunc(GL_LESS));                               // Set the function used with depth testing.
	GLCall(glEnable(GL_BLEND));                                 // Enable blending for alpha channels.
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));  // Set blend function.  This is the standard setting.
	GLCall(glClearColor(1.f, 0.0f, 1.0f, 1.0f));				// Set the color to which OpenGL clears.

	// Compiles the shaders used by the renderer and the FBO.
	Renderer::compileShaders();
	// Loads the default font to be used by the renderer.
	Renderer::loadDefaultFont();

	// Default scenes.
	//generateDefaultScenes();
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
//  Textures.																																	//
//==============================================================================================================================================//

void Renderer::useFont(const Font& font)
{
	m_defaultFont = std::make_unique<Font>(font);
}

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
	samplers.reserve(textureCount);
	for (int i = 1; i < textureCount; i++) 
	{ 
		samplers.push_back(i); 
	}

	// Prepare shader.
	GLCall(auto loc = glGetUniformLocation(m_shaders["TextureShader"]->m_rendererID, "f_textures"));
	GLCall(glUniform1iv(loc, textureCount, samplers.data()));

	// Load font.
	GLCall(glBindTextureUnit(1, m_defaultFont->textureID));

	// Load all of the other textures.
	int count = 2;
	for (auto& [name, texture] : scene->m_textures)
	{
		GLCall(glBindTextureUnit(count++, texture->m_rendererID));
	}
}

//==============================================================================================================================================//
//  Utilities.																																	//
//==============================================================================================================================================//

void Renderer::clearColor() 
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::finish() 
{
	GLCall(glFinish());
}

void Renderer::flush() 
{
	GLCall(glFlush());
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
	GLCall(glViewport(0, 0, (int)viewport.x, (int)viewport.y));
}

void Renderer::setViewport(const glm::vec4& viewport)
{
	GLCall(glViewport((int)viewport[0], (int)viewport[1], (int)viewport[2], (int)viewport[3]));
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
