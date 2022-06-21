#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <vector>
#include <memory>
#include "glm/glm.hpp"
#include <string>
#include <map>
#include <unordered_map>
#include "yaml-cpp/yaml.h"
#include "OpenGL/Buffers/FrameBufferObjectGL.h"

//==============================================================================================================================================//
//  Forward Declerations.																														//
//==============================================================================================================================================//

class IPrimitive;
class Scene;
class Polygon2DTextured;
class Circle;
class LineSegment;
class Text;
class Texture;
class Shader;
class Polygon2D;
class Entity;
class VertexData;
class VertexDataTextured;
class VertexDataCircle;
class IVertexArrayObject;
class PolyLine;
class FrameBufferObject;
class VertexArrayObject;
class IGraphicsPrimitivesBuffer;

template<typename VertexType>
class GraphicsTrianglesBuffer;
template<typename VertexType>
class GraphicsLinesBuffer;

struct Ind3;
struct Font;

typedef unsigned int GLenum;

enum class FrameBufferAttachmentSlot : GLenum;

//==============================================================================================================================================//
//  Rendering.																																	//
//==============================================================================================================================================//

// Contains stats regarding the renderer.
struct RendererData 
{
	// Data.
	unsigned drawCalls = 0;
	unsigned renderPasses = 0;

	// Methods.
	inline void drawCall()	 { drawCalls++;	   }
	inline void renderPass() { renderPasses++; }
	inline void clear() 
	{
		drawCalls = 0;
		renderPasses = 0;
	}
};
//==============================================================================================================================================//
//  Renderer Class.																																//
//==============================================================================================================================================//

static class Renderer
{
public:

	// ------------------- //
	//  U T I L I T I E S  //
	// ------------------- //

	// Set up the renderer and init resources.
	static void initialise();
	// Clears resources.
	static void shutdown();
	// Use the font provided.
	static void useFont(const Font& font);
	// Remove the primitive from the scene.
	static void remove(IPrimitive* primitive);
	// Force the GPU commands.
	static void finish();
	// Flush the GPU commands.
	static void flush();
	// Controls what gets rendered in the pipeline.
	inline static std::unordered_map<std::string, bool> s_pipelineControls;

	// ------------- //
	//  S C E N E S  //
	// ------------- //

	// Bind a new scene.
	static void bindScene(Scene* scene);
	// Return the currently bound scene.
	static Scene* getScene();
	// Render the currently bound scene.
	static void renderScene();
	// Render the specified scene.
	static void renderScene(Scene* scene);
	// Unbind the scene.
	static void unbindScene();
	// Prepare for scene destruction.
	static void initSceneDestruction(Scene* scene);
	// Done with scene destruction.
	static void doneSceneDestruction();
	// Bind a scene, but store the currently bound scene.
	static void storeAndBindScene(Scene* scene);
	// Unbind the scene and bind the stored scene.
	static void restoreAndUnbindScene();
	// The set MSAA level.
	inline static FrameBufferSamples MSAA = FrameBufferSamples::MSAA4;

	// --------------------------- //
	//  2 D   P R I M I T I V E S  //
	// --------------------------- //

	// Add primitives to the bound scene.

	static std::unique_ptr<Polygon2D>&& addPolygon2D(const std::vector<glm::vec3>& vertices, Entity* parent = nullptr);
	static std::unique_ptr<Polygon2D>&& addPolygon2D(const std::vector<glm::vec3>& vertices, const glm::vec4& color, Entity* parent = nullptr);
	static std::unique_ptr<PolyLine>&& addPolygon2DClear(const std::vector<glm::vec2>& vertices, float thickness = 0.014f, Entity* parent = nullptr, glm::vec4 colour = {0.f, 0.f, 0.f, 1.f});
	static std::unique_ptr<Circle>&& addCircle2D(const glm::vec3& center, float radius, const glm::vec4& color, float thickness = -1.f, float fade = 0.0f, Entity* parent = nullptr);
	static std::unique_ptr<Circle>&& addCircle2D(const glm::vec2& center, float radius, const glm::vec4& color, float thickness = -1.f, float fade = 0.0f, Entity* parent = nullptr);
	static std::unique_ptr<LineSegment>&& addLineSegment2D(const glm::vec2& start, const glm::vec2& end, float thickness = 0.001f, const glm::vec4& colour = { 0.f, 0.f, 0.f, 1.f }, Entity* parent = nullptr);
	static std::unique_ptr<Text>&& addText2D(const std::string& text, const glm::vec3& position, const glm::vec4& color, float scale, const std::string& horizontalAlignment = "L", const std::string& verticalAlignment = "B", Entity* parent = nullptr);
	static std::unique_ptr<Text>&& addText2D(const std::string& text, const glm::vec2& position, const glm::vec4& color, float scale, const std::string& horizontalAlignment = "L", const std::string& verticalAlignment = "B", Entity* parent = nullptr);
	static std::unique_ptr<PolyLine>&& addPolyLine(const std::vector<glm::vec2>& vertices, float thickness = 0.014f, const glm::vec4& color = {0.f, 0.f, 0.f, 1.f}, bool rounded = true, Entity* parent = nullptr);

	// Add primitives from file.

	static std::unique_ptr<Text>&& addText2D(const YAML::Node& node, Entity* parent = nullptr);
	static std::unique_ptr<LineSegment>&& addLineSegment2D(const YAML::Node& node, Entity* parent = nullptr);
	static std::unique_ptr<Polygon2D>&& addPolygon2D(const YAML::Node& node, Entity* parent = nullptr);
	static std::unique_ptr<Circle>&& addCircle2D(const YAML::Node& node, Entity* parent = nullptr);
	static std::unique_ptr<PolyLine>&& addPolyLine(const YAML::Node& node, Entity* parent = nullptr);

	// Add primitives that already exist on CPU.

	static void add(Polygon2D& polygon);
	static void add(PolyLine& polyLine);
	static void add(Circle& circle);
	static void add(LineSegment lineSegment);
	static void add(Text& text);

	// --------------------------- //
	//  3 D   P R I M I T I V E S  //
	// --------------------------- //

	// TODO!

	// ----------------- //
	//  T E X T U R E S  //
	// ----------------- //

	// Generates a texture that is used in OpenGL, given the resource ID.
	static Texture* generateTexture(unsigned resourceID, const std::string& name);
	// Returns the texture generated from the resource ID.
	// If a texture has not been generated, generate a new one.
	static Texture* getTexture(const std::string& name);
	// Add an existing texture to the scene.
	static void addTexture(const Texture& texture);

	// ----------------------------- //
	//  D E F A U L T   S C E N E S  //
	// ----------------------------- //

	// Generate the default scenes.
	static void generateDefaultScenes();
	// Get the default 2D scene texture.
	static unsigned getDefault2DSceneTexture();


private:

	friend class Application;
	friend class Scene;
	friend class FrameBufferObject;

	// Prevent instances from being created.
	inline Renderer() = delete;

	// --------------- //
	//  D R A W I N G  //
	// --------------- //

	// Buffers.
	static void drawBufferIndexed(IGraphicsPrimitivesBuffer& vao);
	static void drawBufferIndexedForcePrimitive(IGraphicsPrimitivesBuffer& vao, unsigned primitive);

	// Textures.
	static void drawTextureOverFBOAttachment(FrameBufferObject& FBO, FrameBufferAttachmentSlot slot, unsigned texture, Shader* shader);
	static std::unique_ptr<GraphicsTrianglesBuffer<VertexDataTextured>> s_unitQuad;
	static void createUnitQuad();

	// --------------------- //
	//  P R O C E S S I N G  //
	// --------------------- //

	static void resolveMSAA(FrameBufferObject& sourceFBO, FrameBufferAttachmentSlot sourceSlot, FrameBufferObject& destFBO, FrameBufferAttachmentSlot destSlot);
	static void blit(FrameBufferObject& sourceFBO, FrameBufferAttachmentSlot sourceSlot, FrameBufferObject& destFBO, FrameBufferAttachmentSlot destSlot, GLenum filter = GL_NEAREST, GLenum type = GL_COLOR_BUFFER_BIT);
	static void blitDepth(FrameBufferObject& sourceFBO, FrameBufferObject& destFBO);
	static void blitStencil(FrameBufferObject& sourceFBO, FrameBufferObject& destFBO);
	static void blitDepthStencil(FrameBufferObject& sourceFBO, FrameBufferObject& destFBO);

	// ------------------- //
	//  U T I L I T I E S  //
	// ------------------- //

	// Statically compile all of the shaders.
	static void compileShaders();
	// Load the default font to the renderer.
	static void loadDefaultFont();
	// Load the current scenes textures to OpenGL.
	static void loadTextures(Scene* scene);

	// Enables the OpenGL attribute.
	static void enable(unsigned attribute);
	// Disables the OpenGL attribute.
	static void disable(unsigned attribute);
	// Sets the OpenGL depth function.
	static void setDepthFunc(unsigned function);
	// Set the OpenGL viewport.  Assumes it starts from (0,0).
	static void setViewport(const glm::vec2& viewport);
	// Sets the OpenGL viewport.  Does not assume a start from (0,0).
	static void setViewport(const glm::vec4& viewport);
	// Set the clear color.
	static void setClearColor(const glm::vec4& color);
	static void clearWithColor(const glm::vec4& color);
	// Set the line width (in pixels) of a GL_LINE.
	static void setLineSize(int size);

	// Clearing.
	static void clear(int bitplane);
	static void clearColor();
	static void clearDepth();
	static void clearStencil();
	static void clearDepthStencil();
	static void clearAll();

	// ------------- //
	//  S C E N E S  //
	// ------------- //

	// The scene rendered to.
	static Scene* s_scene;
	// Scene stored when another scene is being destroyed or bound.
	static std::vector<Scene*> s_storedScenes;
	// The 2D Rendering pipeline.
	static void renderingPipeline2D(Scene* scene);
	static void renderingPipeline2DMSAA(Scene* scene);
	// Render a 2D scene's gemeometry.
	static void geometryPass2D(Scene* scene);
	// Render the object outlining.
	static void objectOutliningPass2D(Scene* scene);
	// The 3D Rendering pipeline.
	static void renderingPipeline3D(Scene* scene);
	// Render a 3D scene's gemeometry.
	static void geometryPass3D(Scene* scene);
	// Draw the scene background.
	static void backgroundPass(Scene* scene);
	// Render the grid.
	static void gridPass(Scene* scene);

	// ------------------- //
	//  V A R I A B L E S  //
	// ------------------- //

	// The default font used in the scenes.
	static std::unique_ptr<Font> s_defaultFont;
	// Shaders used by the renderer.
	static std::map<std::string, std::unique_ptr<Shader>> s_shaders;
	// The default background color.
	inline static glm::vec4 backgroundColor = { (float)66 / 255, (float)66 / 255, (float)68 / 255, 1.f };
	inline static glm::vec4 defaultClearColor = { 1.f, 0.f, 1.f, 1.f };

	// ---------------------------- //
	// D E F A U L T   S C E N E S  //
	// ---------------------------- //

	// These scenes are used to render as the default.
	// For example, when FBO resources are deleted, instead of rendering
	// a black texture, we render these senes.
	static std::unique_ptr<Scene> m_default2DScene;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//