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
	

	//m_activeComponent = std::make_shared<Component2D>();
}

// Destructor.
Design2DEngineGL::~Design2DEngineGL()
{
	// Call base destructor.
	Base2DEngineGL::~Base2DEngineGL();

	// Delete helpers.
	m_components.empty();

	//delete polygon;
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
	/*m_basicShader->bind();
	for (int i = 0; i < m_components.size(); i++) {
		m_components[i]->render();
	}
	if (m_activeComponent != NULL) {
		m_activeComponent->render();
		m_textureShader->bind();
		m_activeComponent->textVAO->render();
	}*/

	// --------------- //
	//  C L E A N U P  //
	// --------------- //

	// Stop rendering to the FBO.
	m_frameBuffer->unbind();
}

//=============================================================================================================================================//
//  Events.																																	   //
//=============================================================================================================================================//

void Design2DEngineGL::resizeEvent(int width, int height)
{
	// Call base engine resize event.
	Base2DEngineGL::resizeEvent(width, height);
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
