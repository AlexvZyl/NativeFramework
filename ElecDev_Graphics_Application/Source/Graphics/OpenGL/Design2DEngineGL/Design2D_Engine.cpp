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
#include <GLFW/glfw3.h>

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

// Constructor.
Design2DEngineGL::Design2DEngineGL(GUIState* guiState) 
	: Base2DEngineGL(guiState)
{
	std::cout << blue << "\n[OPENGL] [INFO] : " << white << "Design 2D engine starting...";

	vao = new VertexArrayObject(GL_LINES);
	VertexData v1(
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1
	);
	VertexData v2(
		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1
	);
	std::vector < VertexData > vec1 = { v1, v2 };
	vao->appendDataCPU(vec1);
	vao->updateGPU();
	component = new Component2D();

	std::cout << blue << "\n[OPENGL] [INFO] : " << white << "Design 2D engine done.";
}

// Destructor.
Design2DEngineGL::~Design2DEngineGL()
{
	// Call base destructor.
	Base2DEngineGL::~Base2DEngineGL();

	delete vao;
	delete component;
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
	m_basicShader->bind();
	vao->render();
	component->render();

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
