#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <memory>

//==============================================================================================================================================//
//  Forward Declerations.																														//
//==============================================================================================================================================//

template <typename VertexType>
class Primitive;
template <typename VertexType>
class Polygon2D;

class Scene;
class Polygon2DTextured;
class Circle2D;
class LineSegment2D;
class Text2D;
class Texture;
class Shader;

//==============================================================================================================================================//
//  Renderer Class.																																//
//==============================================================================================================================================//

static class Renderer
{
public:

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

	/*static Polygon2D* addPolygon2D();
	static Polygon2DTextured* addPolygon2DTextured();
	static Circle2D* addCircle2D();
	static LineSegment2D* addLineSegment2D();
	static Text2D* addText2D();*/
	

	// --------------------------- //
	//  3 D   P R I M I T I V E S  //
	// --------------------------- //

	// ----------------- //
	//  T E X T U R E S  //
	// ----------------- //

	static Texture* generateTexture(unsigned resourceID);
	static Texture* getTexture(unsigned resourceID);

	
	// Remove a primitive from the entity ID.
	static void removePrimitive(unsigned entityID);
	// Return a pointer to the primtive given the entity ID.
	//Primitive* getPrimitive(unsigned entityID);

	// Statically compile all of the shaders.
	static void compileShaders();

private:

	// Prevent instances from being created.
	Renderer() {}

	// The scene rendered to.
	static Scene* m_scene;

	// Render a scene with a 2D camera.
	static void render2DScene(Scene* scene);
	// Render a scene with a 3D camera.
	static void render3DScene(Scene* scene);

	// --------------- //
	//  S H A D E R S  //
	// --------------- //

	// Shaders.
	static std::unique_ptr<Shader> m_backgroundShader2D;
	static std::unique_ptr<Shader> m_backgroundShader3D;
	static std::unique_ptr<Shader> m_basicShader;
	static std::unique_ptr<Shader> m_textureShader;
	static std::unique_ptr<Shader> m_circleShader;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
