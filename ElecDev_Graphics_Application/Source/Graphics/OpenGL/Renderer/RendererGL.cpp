//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "glad/glad.h"
#include "Resources/ResourceHandler.h"
#include "OpenGL/ShaderGL.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "OpenGL/ErrorHandlerGL.h"
#include "OpenGL/Buffers/FrameBufferObjectGL.h"
#include "OpenGL/Buffers/VertexArrayObjectGL.h"
#include "OpenGL/Primitives/Vertex.h"
#include "Graphics/Fonts/FontLoader.h"
#include "Application/Application.h"

//==============================================================================================================================================//
//  Static Inisialisation.																														//
//==============================================================================================================================================//

std::map<std::string, std::unique_ptr<Shader>> Renderer::s_shaders;
Scene* Renderer::s_scene = nullptr;
Scene* Renderer::s_storedScene = nullptr;
std::unique_ptr<Font> Renderer::s_defaultFont = nullptr;
std::unique_ptr<Scene> Renderer::m_default2DScene = nullptr;
std::unique_ptr<VertexArrayObject<VertexDataTextured>> Renderer::s_unitQuad = nullptr;

//==============================================================================================================================================//
//  Setup.																																		//
//==============================================================================================================================================//

void Renderer::shutdown()
{
	s_shaders.clear();
	m_default2DScene.reset();
	s_defaultFont.reset();
}

void Renderer::initialise()
{
	// OpenGL settings.
	GLCall(glEnable(GL_MULTISAMPLE));                           // Enables MSAA.
	GLCall(glEnable(GL_DEPTH_TEST));                            // Enables depth testing for the OpenGL scenes.
	GLCall(glEnable(GL_STENCIL_TEST));							// Enable stencil buffer.
	GLCall(glDepthFunc(GL_LESS));                               // Set the function used with depth testing.
	GLCall(glEnable(GL_BLEND));                                 // Enable blending for alpha channels.
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));  // Set blend function.  This is the standard setting.
	Renderer::setClearColor(Renderer::baseColor);

	// Compiles the shaders used by the renderer and the FBO.
	Renderer::compileShaders();
	// Loads the default font to be used by the renderer.
	Renderer::loadDefaultFont();

	// Setup pipeline controls.
	s_pipelineControls.insert({ "Background"		, true });
	s_pipelineControls.insert({ "Geometry"			, true });
	s_pipelineControls.insert({ "Outline"			, true });
	s_pipelineControls.insert({ "OutlinePostProc"   , true });

	// Used for texture rendering.
	Renderer::createUnitQuad();
}

void Renderer::loadDefaultFont()
{
	s_defaultFont = std::make_unique<Font>(msdfLoadFont(ARIAL_NORMAL_JSON, ARIAL_NORMAL_PNG));
}

void Renderer::compileShaders()
{
	// Renderer shaders.
	s_shaders.insert({ "BackgroundShader"		, std::make_unique<Shader>(BACKGROUND_SHADER) });
	s_shaders.insert({ "BasicShader"			, std::make_unique<Shader>(BASIC_SHADER) });
	s_shaders.insert({ "TextureShader"			, std::make_unique<Shader>(TEXTURE_SHADER) });
	s_shaders.insert({ "CircleShader"			, std::make_unique<Shader>(CIRCLE_SHADER) });
	s_shaders.insert({ "OutlineShader"			, std::make_unique<Shader>(OUTLINE_SHADER) });
	s_shaders.insert({ "OutlineShaderTextures"	, std::make_unique<Shader>(OUTLINE_SHADER_TEXTURES) });
	s_shaders.insert({ "OutlineShaderCircle"	, std::make_unique<Shader>(OUTLINE_SHADER_CIRCLE) });
	s_shaders.insert({ "StaticTextureShader"	, std::make_unique<Shader>(STATIC_TEXTURE_SHADER) });
	s_shaders.insert({ "OutlineBackgroundShader", std::make_unique<Shader>(OUTLINE_SHADER_BACKGROUND) });
	s_shaders.insert({ "OutlinePostProc"		, std::make_unique<Shader>(OUTLINE_SHADER_POSTPROC) });

	Shader* shader;
	int samplers[1] = { 0 };

	// Setup static shader samplers.
	shader = s_shaders["StaticTextureShader"].get();
	shader->bind();
	GLCall(auto loc = glGetUniformLocation(shader->m_rendererID, "f_textures"));
	GLCall(glUniform1iv(loc, 1, samplers));

	// Set post processing samplers.
	shader = s_shaders["OutlinePostProc"].get();
	shader->bind();
	GLCall(loc = glGetUniformLocation(shader->m_rendererID, "f_textures"));
	GLCall(glUniform1iv(loc, 1, samplers));
}

void Renderer::createUnitQuad() 
{
	// Create VAO.
	s_unitQuad = std::make_unique<VertexArrayObject<VertexDataTextured>>(GL_TRIANGLES);
	s_unitQuad->setBufferIncrementSize(4);

	// Vertex data.
	s_unitQuad->m_vertexCPU.emplace_back(VertexDataTextured(glm::vec3(-1.f, -1.f, 0.f), glm::vec4(0.f, 0.f, 0.f, 0.f), glm::vec2(0.f, 0.f), 0.f, 0));
	s_unitQuad->m_vertexCPU.emplace_back(VertexDataTextured(glm::vec3(-1.f, 1.f, 0.f), glm::vec4(0.f, 0.f, 0.f, 0.f), glm::vec2(0.f, 1.f), 0.f, 0));
	s_unitQuad->m_vertexCPU.emplace_back(VertexDataTextured(glm::vec3(1.f, 1.f, 0.f), glm::vec4(0.f, 0.f, 0.f, 0.f), glm::vec2(1.f, 1.f), 0.f, 0));
	s_unitQuad->m_vertexCPU.emplace_back(VertexDataTextured(glm::vec3(1.f, -1.f, 0.f), glm::vec4(0.f, 0.f, 0.f, 0.f), glm::vec2(1.f, 0.f), 0.f, 0));
	s_unitQuad->m_vertexCount += 4;

	// Index data.
	s_unitQuad->m_indexCPU.insert(s_unitQuad->m_indexCPU.end(), { 0,1,2, 2,3,0 });
	s_unitQuad->m_indexCount += 6;

	// Data will be loaded upon first resize.
	s_unitQuad->queryBufferResize();
}

//==============================================================================================================================================//
//  Textures.																																	//
//==============================================================================================================================================//

void Renderer::useFont(const Font& font)
{
	s_defaultFont = std::make_unique<Font>(font);
}

Texture* Renderer::generateTexture(unsigned resourceID, const std::string& name)
{
	s_scene->m_textures.insert({ name, std::make_unique<Texture>(resourceID, name) });
	return s_scene->m_textures[name].get();
}

void Renderer::addTexture(const Texture& texture) 
{
	s_scene->m_textures.insert({texture.m_name, std::make_unique<Texture>(texture)});
}

Texture* Renderer::getTexture(const std::string& name)
{
	return s_scene->m_textures[name].get();
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
	GLCall(auto loc = glGetUniformLocation(s_shaders["TextureShader"]->m_rendererID, "f_textures"));
	GLCall(glUniform1iv(loc, textureCount, samplers.data()));

	// Load font.
	GLCall(glBindTextureUnit(1, s_defaultFont->textureID));

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

void Renderer::setClearColor(const glm::vec4& color) 
{
	GLCall(glClearColor(color.r, color.g, color.b, color.a));
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
