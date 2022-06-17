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
#include "OpenGL/SceneGL.h"
#include "Graphics/Fonts/FontLoader.h"
#include "Application/Application.h"
#include "Utilities/Logger/Logger.h"
#include "OpenGL/Buffers/GraphicsPrimitivesBuffersGL.h"

//==============================================================================================================================================//
//  Static Inisialisation.																														//
//==============================================================================================================================================//

std::map<std::string, std::unique_ptr<Shader>> Renderer::s_shaders;
Scene* Renderer::s_scene = nullptr;
std::vector<Scene*> Renderer::s_storedScenes;
std::unique_ptr<Font> Renderer::s_defaultFont = nullptr;
std::unique_ptr<Scene> Renderer::m_default2DScene = nullptr;
std::unique_ptr<GraphicsTrianglesBuffer<VertexDataTextured>> Renderer::s_unitQuad = nullptr;

//==============================================================================================================================================//
//  Setup.																																		//
//==============================================================================================================================================//

void Renderer::shutdown()
{
	s_shaders.clear();
	m_default2DScene.reset();
	s_defaultFont.reset();
	s_unitQuad.reset();
}

void Renderer::initialise()
{
	LUMEN_LOG_INFO("Initialising...", "OpenGL Renderer");

	// OpenGL settings.
	GLCall(glEnable(GL_MULTISAMPLE));                           // Enables MSAA.
	GLCall(glEnable(GL_DEPTH_TEST));                            // Enables depth testing for the OpenGL scenes.
	GLCall(glEnable(GL_STENCIL_TEST));							// Enable stencil buffer.
	GLCall(glDepthFunc(GL_LESS));                               // Set the function used with depth testing.
	GLCall(glEnable(GL_BLEND));                                 // Enable blending for alpha channels.
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));  // Set blend function.  This is the standard setting.
	Renderer::setClearColor(Renderer::baseColor);

	// Initial setup.
	Renderer::compileShaders();
	Renderer::loadDefaultFont();
	// Setup pipeline controls.
	s_pipelineControls["Background"		] = true;
	s_pipelineControls["Geometry"		] = true;
	s_pipelineControls["Outline"		] = true;
	s_pipelineControls["OutlinePostProc"] = true;
	s_pipelineControls["Grid"			] = true;
	Renderer::createUnitQuad();

	LUMEN_LOG_INFO("Initialised.", "OpenGL Renderer");
}

void Renderer::loadDefaultFont()
{
	s_defaultFont = std::make_unique<Font>(msdfLoadFont(ARIAL_NORMAL_JSON, ARIAL_NORMAL_PNG));
}

void Renderer::compileShaders()
{
	// Compile shaders.
	s_shaders["BackgroundShader"		] = std::make_unique<Shader>(BACKGROUND_SHADER);
	s_shaders["BasicShader"				] = std::make_unique<Shader>(BASIC_SHADER);
	s_shaders["TextureShader"			] = std::make_unique<Shader>(TEXTURE_SHADER);
	s_shaders["CircleShader"			] = std::make_unique<Shader>(CIRCLE_SHADER);
	s_shaders["StaticTextureShader"		] = std::make_unique<Shader>(STATIC_TEXTURE_SHADER);
	s_shaders["OutlinePostProc"			] = std::make_unique<Shader>(OUTLINE_SHADER_POSTPROC);
	s_shaders["MSAA"					] = std::make_unique<Shader>(MSAA_SHADER);

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
	s_unitQuad = std::make_unique<GraphicsTrianglesBuffer<VertexDataTextured>>();
	s_unitQuad->setCapacityIncrements(2);

	// Vertex data.
	const VertexDataTextured vertices[4] = {
		VertexDataTextured({ -1.f, -1.f,  0.f }, { 1.f, 0.f, 1.f, 1.f }, { 0.f, 0.f }, 0.f),
		VertexDataTextured({ -1.f,  1.f,  0.f }, { 1.f, 0.f, 1.f, 1.f }, { 0.f, 1.f }, 0.f),
		VertexDataTextured({  1.f,  1.f,  0.f }, { 1.f, 0.f, 1.f, 1.f }, { 1.f, 1.f }, 0.f),
		VertexDataTextured({  1.f, -1.f,  0.f }, { 1.f, 0.f, 1.f, 1.f }, { 1.f, 0.f }, 0.f)
	};

	// Index data.
	const UInt3 indices[2] = {
		UInt3{ 0, 1, 2 },
		UInt3{ 2, 3, 0 }
	};

	// Push data.
	s_unitQuad->push(vertices, 4, indices, 2);
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
	int textureCount = (int)scene->m_textures.size() + 2;
	std::vector<int> samplers = { 0 };
	samplers.reserve(textureCount);
	for (int i = 1; i < textureCount; i++) 
		samplers.push_back(i); 

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

void Renderer::clearColor()								{ GLCall(glClear(GL_COLOR_BUFFER_BIT)); }
void Renderer::finish()									{ GLCall(glFinish()); }
void Renderer::flush()									{ GLCall(glFlush()); }
void Renderer::enable(unsigned attribute)				{ GLCall(glEnable(attribute)); }
void Renderer::disable(unsigned attribute)				{ GLCall(glDisable(attribute)); }
void Renderer::setDepthFunc(unsigned function)			{ GLCall(glDepthFunc(function)); }
void Renderer::setViewport(const glm::vec2& viewport)	{ GLCall(glViewport(0, 0, (int)viewport.x, (int)viewport.y)); }
void Renderer::setViewport(const glm::vec4& viewport)	{ GLCall(glViewport((int)viewport[0], (int)viewport[1], (int)viewport[2], (int)viewport[3])); }
void Renderer::setClearColor(const glm::vec4& color)	{ GLCall(glClearColor(color.r, color.g, color.b, color.a)); }
void Renderer::setLineSize(int size)					{ GLCall(glLineWidth((float)size)); }
void Renderer::clear(int bitplane)						{ GLCall(glClear(bitplane)); }

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
