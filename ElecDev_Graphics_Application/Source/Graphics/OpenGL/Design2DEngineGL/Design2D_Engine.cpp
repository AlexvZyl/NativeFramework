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

	// Done.
	std::cout << blue << "\n[OPENGL] [INFO] : " << white << "Design 2D engine done.";
	m_guiState->design_engine = this;
}

// Destructor.
Design2DEngineGL::~Design2DEngineGL()
{
	// Delete components.
	m_circuit->m_components.empty();
	m_circuit->m_components.shrink_to_fit();
	m_circuit->m_cables.empty();
	m_circuit->m_cables.shrink_to_fit();
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
