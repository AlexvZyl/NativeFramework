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
	// Statically compile all of the shaders.
	static void compileShaders();

	// ------------- //
	//  S C E N E S  //
	// ------------- //

	// Bind a new scene.
	static void bindScene(Scene* scene);
	// Render the currently bound scene.
	static void renderScene();
	// Render the specified scene.
	static void renderScene(Scene* scene);
	// Unbind the scene.
	static void unbindScene();

	// --------------------------- //
	//  2 D   P R I M I T I V E S  //
	// --------------------------- //

	// Polygons.
	static Polygon2D* addPolygon2D(std::vector<glm::vec3> vertices, Entity* parent);
	//static Polygon2DTextured* addPolygon2DTextured();
	// Circles.
	static Circle* addCircle2D(glm::vec3& center, float radius, glm::vec4& color, float thickness, float fade, Entity* parent);
	static Circle* addCircle2D(glm::vec2& center, float radius, glm::vec4& color, float thickness, float fade, Entity* parent);
	// Lines.
	static LineSegment* addLineSegment2D(glm::vec2 start, glm::vec2 end, Entity* parent, float thickness = 0.001f, glm::vec4 colour = { 0.f, 0.f, 0.f, 1.f });
	// Text.
	static Text* addText2D(std::string text, glm::vec3& position, glm::vec4& color, float scale, Entity* parent, std::string horizontalAlignment = "L", std::string verticalAlignment = "B");
	
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
	
	// Remove a primitive from the entity ID.
	static void removePrimitive(unsigned entityID);
	// Return a pointer to the primtive given the entity ID.
	//Primitive* getPrimitive(unsigned entityID);

private:

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
