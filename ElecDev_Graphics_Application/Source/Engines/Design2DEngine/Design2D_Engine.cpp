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
#include "OpenGL/Entities/Polygon.h"
#include "OpenGL/Entities/Circle.h"
#include "OpenGL/Buffers/FrameBufferObjectGL.h"
#include "OpenGL/ShaderGL.h"
#include "OpenGL/Buffers/VertexArrayObjectGL.h"
#include "OpenGL/Entities/Primitive.h"
// Window.
#include <GLFW/glfw3.h>
// Design components.
#include "Peripherals/Circuit.h"

// For Testing.
#include "OpenGL/RendererGL.h"

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

// Constructor.
Design2DEngine::Design2DEngine(GUIState* guiState, std::string contextName)
	: Base2DEngine(guiState, contextName)
{
	// Starting.
	std::cout << blue << "\n[OPENGL] [INFO] : " << white << "Design 2D engine starting...";

	// Default circuit (In this case handled like a default scene).
	m_circuit = std::make_shared<Circuit>(m_contextName, "AE");

	m_guiState->design_engine = this;

	Renderer::bindScene(m_scene.get());
	LineSegment* line =  Renderer::addLineSegment2D(glm::vec2(0,0), glm::vec2(1,1), 1.f);
	line->setLayer(0.5f);
	Renderer::addLineSegment2D(glm::vec2(0, 0), glm::vec2(1, 1), 1.f);
	Renderer::addLineSegment2D(glm::vec2(0, 0), glm::vec2(1, 1), 1.f);
	Renderer::addLineSegment2D(glm::vec2(0, 0), glm::vec2(1, 1), 1.f);
	Renderer::addLineSegment2D(glm::vec2(0, 0), glm::vec2(1, 1), 1.f);
	Renderer::addLineSegment2D(glm::vec2(0, 0), glm::vec2(1, 1), 1.f);
	Renderer::addLineSegment2D(glm::vec2(0, 0), glm::vec2(1, 1), 1.f);
	Renderer::addLineSegment2D(glm::vec2(0, 0), glm::vec2(1, 1), 1.f);
	glm::vec2 ctr(0.f, 0.f);
	glm::vec4 black(0.f, 0.f, 0.f, 1.f);
	Circle* circle = Renderer::addCircle2D(ctr, 0.2, black);

	std::vector<glm::vec3> verts =
	{
		glm::vec3(-0.5f, -0.5f, 0.f),
		glm::vec3(-0.5f, -1.f, 0.f),
		glm::vec3(-1.f, -1.f, 0.f),
		glm::vec3(-1.f, -0.5f, 0.f)
	};
	Polygon2D* poly = Renderer::addPolygon2D(verts);
	poly->setLayer(0.5f);


	// Done.
	std::cout << blue << "\n[OPENGL] [INFO] : " << white << "Design 2D engine done.";
}

// Destructor.
Design2DEngine::~Design2DEngine()
{

}

//=============================================================================================================================================//
//  Events.																																	   //
//=============================================================================================================================================//

void Design2DEngine::resizeEvent(float width, float height)
{
	// Call base engine resize event.
	Base2DEngine::resizeEvent(width, height);
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//


