#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <vector>
#include <memory>
#include "glm.hpp"
#include <string>
#include <map>

//==============================================================================================================================================//
//  Forward Declerations.																														//
//==============================================================================================================================================//

template <typename VertexType>
class Primitive;

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

struct Font;

//==============================================================================================================================================//
//  Renderer Class.																																//
//==============================================================================================================================================//

static class Renderer
{
public:

	// ------------------- //
	//  U T I L I T I E S  //
	// ------------------- //

	// Set up the renderer.
	static void initialise();
	// Use the font provided.
	static void useFont(Font* font);
	// Remove the primitive from the scene.
	static void remove(Primitive<VertexData>* primitive);
	// Remove the primitive from the scene.
	static void remove(Primitive<VertexDataTextured>* primitive);
	// Remove the primitive from the scene.
	static void remove(Primitive<VertexDataCircle>* primitive);

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

	// --------------------------- //
	//  2 D   P R I M I T I V E S  //
	// --------------------------- //

	// Add a filled 2D polygon.
	static Polygon2D* addPolygon2D(std::vector<glm::vec3> vertices, Entity* parent = nullptr);
	// Add a clear 2D polygon.
	static Polygon2D* addPolygon2DClear(std::vector<glm::vec3> vertices, Entity* parent = nullptr);
	// Add a circle.
	static Circle* addCircle2D(glm::vec3& center, float radius, glm::vec4& color, float thickness, float fade, Entity* parent = nullptr);
	// Add a circle.
	static Circle* addCircle2D(glm::vec2& center, float radius, glm::vec4& color, float thickness, float fade, Entity* parent = nullptr);
	// Add a singular line segment.
	static LineSegment* addLineSegment2D(glm::vec2 start, glm::vec2 end, float thickness = 0.001f, glm::vec4 colour = { 0.f, 0.f, 0.f, 1.f }, Entity* parent = nullptr);
	// Add a 2D text string.
	static Text* addText2D(std::string text, glm::vec3& position, glm::vec4& color, float scale, std::string horizontalAlignment = "L", std::string verticalAlignment = "B", Entity* parent = nullptr);
	
	// --------------------------- //
	//  3 D   P R I M I T I V E S  //
	// --------------------------- //



	// ----------------- //
	//  T E X T U R E S  //
	// ----------------- //

	// Generates a texture that is used in OpenGL, given the resource ID.
	static Texture* generateTexture(unsigned resourceID);
	// Returns the texture generated from the resource ID.
	// If a texture has not been generated, generate a new one.
	static Texture* getTexture(unsigned resourceID);
	
private:

	// ------------------- //
	//  U T I L I T I E S  //
	// ------------------- //

	// Statically compile all of the shaders.
	static void compileShaders();
	// Load the default font to the renderer.
	static void loadDefaultFont();
	// Load the current scenes textures to OpenGL.
	static void loadTextures(Scene* scene);
	// Prevent instances from being created.
	Renderer() {}

	// ------------- //
	//  S C E N E S  //
	// ------------- //

	// The scene rendered to.
	static Scene* m_scene;

	// Render a scene with a 2D camera.
	static void render2DScene(Scene* scene);
	// Render a scene with a 3D camera.
	static void render3DScene(Scene* scene);

	// The default font used in the scenes.
	static std::unique_ptr<Font> m_defaultFont;

	// --------------- //
	//  S H A D E R S  //
	// --------------- //

	static std::map<std::string, std::unique_ptr<Shader>> m_shaders;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//