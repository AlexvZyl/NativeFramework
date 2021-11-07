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
// Entities.
#include "Peripherals/Component2D.h"
#include "CoreGL/Entities/Polygon.h"

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

// Constructor.
Design2DEngineGL::Design2DEngineGL(GUIState* guiState) 
	: Base2DEngineGL(guiState)
{
	std::cout << blue << "\n[OPENGL] [INFO] : " << white << "Design 2D engine starting...";


	glm::vec3 v1(-0.5f, 0.5f, 0.0f);
	glm::vec3 v2(0.5f, 0.5f, 0.0f);
	glm::vec3 v3(0.5f, -0.5f, 0.0f);
	glm::vec3 v4(-0.5f, -0.5f, 0.0f);
	std::vector<glm::vec3> vertices = { v1, v2, v3, v4 };
	polygon = new Polygon2D(vertices, m_trianglesVAO);
	

	std::cout << blue << "\n[OPENGL] [INFO] : " << white << "Design 2D engine done.";
	
	m_components.insert(m_components.begin(), std::make_shared<Component2D>());
	m_activeComponent = m_components[0];
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
	m_basicShader->bind();
	for (int i = 0; i < m_components.size(); i++) {
		m_components[i]->render();
	}

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
