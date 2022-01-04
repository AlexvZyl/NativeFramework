/*
This is where the interactive 2D design engine is implemented.
*/

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

// Class include.
#include "Design2D_Engine.h"
// General.
#include <iostream>
#include <External/Misc/ConsoleColor.h>
// Resource loading.
#include "Resources/ResourceHandler.h"
// Entities.
#include "Peripherals/Component2D.h"
// OpenGL core elements.
#include "CoreGL/Entities/Polygon.h"
#include "CoreGL/Entities/Circle.h"
#include "CoreGL/FrameBufferObjectGL.h"
#include "CoreGL/ShaderHandlerGL.h"
#include "CoreGL/VertexArrayObjectGL.h"
// Window.
#include <GLFW/glfw3.h>
// Design components.
#include "Peripherals/Circuit.h"

// For Testing.
#include "CoreGL/Renderer.h"

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

// Constructor.
Design2DEngineGL::Design2DEngineGL(GUIState* guiState, std::string contextName)
	: Base2DEngineGL(guiState, contextName)
{
	// Starting.
	std::cout << blue << "\n[OPENGL] [INFO] : " << white << "Design 2D engine starting...";

	// Default circuit (In this case handled like a default scene).
	m_circuit = std::make_shared<Circuit>(m_contextName, "AE");

	//// Testing.
	//glm::vec2 ctr2(0, 0);
	//glm::vec2 ctr2a(1, 0);
	//glm::vec2 ctr2b(0, 1);
	//glm::vec3 ctr3(-2,0,0);
	//std::vector<glm::vec3> verts = {
	//	glm::vec3(0,0,0),
	//	glm::vec3(0,1,0),
	//	glm::vec3(1,1,0),
	//	glm::vec3(1,0,0)
	//};
	//glm::vec4 black(0,0,0,1);
	//Renderer::bindScene(m_scene.get());
	//Polygon2D* poly = Renderer::addPolygon2D(verts, nullptr);
	//poly->setColor(black);
	//Circle* circle = Renderer::addCircle2D(ctr2, 1, black, 0.1, 0.005, nullptr);
	//Renderer::addCircle2D(ctr2a, 1, black, 0.1, 1, nullptr);
	//Renderer::addCircle2D(ctr2b, 1, black, 0.5, 0.005, nullptr);
	//Text* text = Renderer::addText2D("Hello World!", ctr3, black, 1);
	//Renderer::removePrimitive(text);

	m_guiState->design_engine = this;

	// Done.
	std::cout << blue << "\n[OPENGL] [INFO] : " << white << "Design 2D engine done.";
}

// Destructor.
Design2DEngineGL::~Design2DEngineGL()
{

}

//=============================================================================================================================================//
//  Events.																																	   //
//=============================================================================================================================================//

void Design2DEngineGL::resizeEvent(float width, float height)
{
	// Call base engine resize event.
	Base2DEngineGL::resizeEvent(width, height);
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
