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

//==============================================================================================================================================//
//  Forward Declerations.																														//
//==============================================================================================================================================//

class PrimitivePtr;
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
class VertexArrayObjectPtr;
class PolyLine;

template<typename VertexType>
class VertexArrayObject;

struct Font;

//==============================================================================================================================================//
//  Data.																																		//
//==============================================================================================================================================//

struct RendererData 
{
	// Data.
	unsigned drawCalls = 0;
	unsigned renderPasses = 0;

	// Methods.
	inline void drawCall()	 { drawCalls++;	   }
	inline void renderPass() { renderPasses++; }
	inline void reset() 
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

	// Clears resources.
	static void shutdown();

	// ------------------- //
	//  U T I L I T I E S  //
	// ------------------- //

	// Set up the renderer.
	static void initialise();
	// Use the font provided.
	static void useFont(const Font& font);
	// Remove the primitive from the scene.
	static void remove(PrimitivePtr* primitive);
	// Clear the rendering context.
	static void clearColor();
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

	// --------------------------- //
	//  2 D   P R I M I T I V E S  //
	// --------------------------- //

	// Add a filled 2D polygon.
	static Polygon2D* addPolygon2D(const std::vector<glm::vec3>& vertices, Entity* parent = nullptr);
	// Add a filled 2D polygon with color.
	static Polygon2D* addPolygon2D(const std::vector<glm::vec3>& vertices, const glm::vec4& color, Entity* parent = nullptr);
	// Add a clear 2D polygon.
	static PolyLine* addPolygon2DClear(const std::vector<glm::vec2>& vertices, float thickness = 0.014f, Entity* parent = nullptr);
	// Add a circle.
	static Circle* addCircle2D(const glm::vec3& center, float radius, const glm::vec4& color, float thickness = 1, float fade = 0.001, Entity* parent = nullptr);
	// Add a circle.
	static Circle* addCircle2D(const glm::vec2& center, float radius, const glm::vec4& color, float thickness = 1, float fade = 0.001, Entity* parent = nullptr);
	// Add a singular line segment.
	static LineSegment* addLineSegment2D(const glm::vec2& start, const glm::vec2& end, float thickness = 0.001f, const glm::vec4& colour = { 0.f, 0.f, 0.f, 1.f }, Entity* parent = nullptr);
	// Add a 2D text string.
	static Text* addText2D(const std::string& text, const glm::vec3& position, const glm::vec4& color, float scale, const std::string& horizontalAlignment = "L", const std::string& verticalAlignment = "B", Entity* parent = nullptr);
	// Add a 2D text string.
	static Text* addText2D(const std::string& text, const glm::vec2& position, const glm::vec4& color, float scale, const std::string& horizontalAlignment = "L", const std::string& verticalAlignment = "B", Entity* parent = nullptr);
	//Add a polyline
	static PolyLine* addPolyLine(const std::vector<glm::vec2>& vertices, float thickness = 0.014f, const glm::vec4& color = {0.f, 0.f, 0.f, 1.f}, bool rounded = true, Entity* parent = nullptr);

	// Add text from a YAML file.
	static Text* addText2D(const YAML::Node& node, Entity* parent = nullptr);
	// Add line segment from a YAML file.
	static LineSegment* addLineSegment2D(const YAML::Node& node, Entity* parent = nullptr);
	// Add Polygon2D from a YAML file.
	static Polygon2D* addPolygon2D(const YAML::Node& node, Entity* parent = nullptr);
	// Add circle from a YAML file.
	static Circle* addCircle2D(const YAML::Node& node, Entity* parent = nullptr);
	// Add a polyline from a YAML file.
	static PolyLine* addPolyLine(const YAML::Node& node, Entity* parent = nullptr);

	
	// --------------------------- //
	//  3 D   P R I M I T I V E S  //
	// --------------------------- //



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
	Renderer() {}

	// --------------- //
	//  D R A W I N G  //
	// --------------- //

	// Buffers.
	static void drawBufferIndexed(VertexArrayObjectPtr* vao);
	static void drawBufferIndexedForcePrimitive(VertexArrayObjectPtr* vao, unsigned primitive);

	// Textures.
	static void drawTextureOverFBOAttachment(FrameBufferObject* FBO, unsigned texture, unsigned attachment, Shader* shader);
	static std::unique_ptr<VertexArrayObject<VertexDataTextured>> s_unitQuad;
	static void createUnitQuad();

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
	// Set the line width (in pixels) of a GL_LINE.
	static void setLineSize(int size);
	// Clears the bits set.
	static void clear(int bitplane);

	// ------------- //
	//  S C E N E S  //
	// ------------- //

	// The scene rendered to.
	static Scene* s_scene;
	// Scene stored when another scene is being destroyed or bound.
	static std::vector<Scene*> s_storedScenes;
	// The 2D Rendering pipeline.
	static void renderingPipeline2D(Scene* scene);
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
	inline static glm::vec4 baseColor = { 1.f, 0.f, 1.f, 1.f };

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