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
// OpenGL core elements.
// Resource loading.
#include "Resources/ResourceHandler.h"
// Window.

// Entities.
#include "Peripherals/Component2D.h"
#include "CoreGL/Entities/Polygon.h"
#include "CoreGL/Entities/Circle.h"
#include "CoreGL/FrameBufferObjectGL.h"
#include "CoreGL/ShaderHandlerGL.h"
#include "CoreGL/VertexArrayObjectGL.h"
#include <GLFW/glfw3.h>
//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

// Constructor.
Design2DEngineGL::Design2DEngineGL(GUIState* guiState) 
	: Base2DEngineGL(guiState)
{
	std::cout << blue << "\n[OPENGL] [INFO] : " << white << "Design 2D engine starting...";


	std::cout << blue << "\n[OPENGL] [INFO] : " << white << "Design 2D engine done.";	

	/*
	glm::vec4 circColor = { 0.f, 0.f, 1.f, 1.f };
	glm::vec3 centre1 = { -0.3f, -0.3f, 0.f };
	glm::vec3 centre2 = { 0.f, 0.f, 0.f };
	glm::vec3 centre3 = { 0.3f, 0.3f, 0.f };
	circ1 = std::make_unique<Circle<VertexDataCircle>>(m_circleEntitiesVAO.get(), centre1, 0.01, circColor, 1.0f, 0.0f, nullptr);
	circ1->update();
	circ2 = std::make_unique<Circle<VertexDataCircle>>(m_circleEntitiesVAO.get(), centre2, 0.01, circColor, 1.0f, 0.0f, nullptr);
	circ2->update();
	circ3 = std::make_unique<Circle<VertexDataCircle>>(m_circleEntitiesVAO.get(), centre3, 0.01, circColor, 1.0f, 0.0f, nullptr);
	circ3->update();
	//*/

}

// Destructor.
Design2DEngineGL::~Design2DEngineGL()
{
	// Call base destructor.
	Base2DEngineGL::~Base2DEngineGL();

	// Delete components.
	m_components.empty();
	m_components.shrink_to_fit();
}

//=============================================================================================================================================//
//  Rendering.																																   //
//=============================================================================================================================================//

void Design2DEngineGL::renderLoop()
{
	// Call the base engine rendering loop.
	Base2DEngineGL::renderLoop();

	// ----------- //
	//  S E T U P  //
	// ----------- //

	// Render to the FBO.
	m_frameBuffer->bind();

	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// Helpers.

	// --------------- //
	//  C L E A N U P  //
	// --------------- //

	// Stop rendering to the FBO.
	m_frameBuffer->unbind();
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
